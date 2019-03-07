#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

class communication {
private:
    uint8_t dere_pin; // data enable/read enable pin number
    uint8_t buf[20];
    uint8_t buf_len; // number of utilized bytes

    enum command_t : uint8_t {
        unknown = 0x00,
        // requests
        setPWMLimitRequest = 0xA0,
        dataRequest = 0xC0,
        toggleLEDRequest = 0xD0,
        ping = 0xE0,
        setPosVoltageRequest = 0xB0,
        setNegVoltageRequest = 0xB1,
        // responses
        dataResponse = 0x80,
        pong = 0xE1,
    };

    command_t comtype = unknown;

    /*
      _peek returns the next byte on the serial buffer without removing it.
      subsequent calls of peek() will return the same byte over and over.
      If no byte is available, the return value will be -1.
    */
    int _peek()
    {
        return Serial.peek();
    }

    /*
       _pop returns the next byte on the serial buffer, removing it afterwards.
       If no byte is available, the return value will be -1.
    */
    int _pop()
    {
        return Serial.read();
    }

    /*
       _get_sync_bytes eats bytes until two successive sync bytes are encountered,
       after which it will stop eating and returns `true`.
       if the serial buffer does not contain two successive sync bytes, `false` will
       be returned.
    */
    bool _get_sync_bytes()
    {
        uint8_t sync = 0;
        for (;;) {
            // once we have two sync bytes, we are successful
            if (sync == 2) {
                // add sync bytes to buffer
                buf[buf_len] = 0xFF;
                buf_len++;
                buf[buf_len] = 0xFF;
                buf_len++;
                return true;
            }
            switch (this->_pop()) {
            case -1:
                // no bytes left and yet no sync
                return false;
            case 0xFF:
                sync++;
                break;
            default:
                // any other byte will reset the counter
                sync = 0;
                break;
            }
        }
    }

    /*
       _get_byte() works like _pop(), but adds the read byte automatically
       to the received buffer.
    */
    bool _get_byte()
    {
        int received = _pop();
        if (received < 0) {
            return false;
        }

        // add byte to buffer
        buf[buf_len] = (uint8_t)received;
        buf_len++;
        return true;
    }

    command_t _get_message_type()
    {
        if (buf_len < 3)
            return (command_t)0x00;

        return (command_t)buf[2];
    }

    /*
       _get_message_length returns the expected message length for a specific message type in bytes,
       including the sync bytes and chcecksum.
    */
    int8_t _get_msg_length(uint8_t msg_id)
    {
        switch (msg_id) {
        case dataResponse:
            return 15;
        case pong:
            return 5;
        default:
            return -1; // error
        }
    }

    bool _read_until_len(int8_t msg_len)
    {
        if (msg_len < 0)
            return false; // error, block processing

        int received;
        for (;;) {
            received = this->_pop();
            if (received < 0)
                // no new byte
                return false;

            buf[buf_len] = (uint8_t)received;
            buf_len++;

            if (buf_len == msg_len)
                // we reached required amount of bytes
                return true;
        }
    }

public:
    enum command_state_t : uint8_t {
        syncing = 0,
        awaiting = 1,
        get_id = 2,
        reading = 3,
        eating = 4,
        verifying = 5,
        pending = 6,
        finished = 7,
        error = 8,
    };

    command_state_t syncstate = syncing;

    communication()
        : dere_pin(13)
    {
        pinMode(dere_pin, OUTPUT);
    };

    communication(uint8_t dere_pin)
        : dere_pin(dere_pin)
    {
        pinMode(dere_pin, OUTPUT);
    };
    ~communication(){};

    void reset()
    {
        syncstate = syncing;
        buf_len = 0;
        read_bytes = 0;
        comtype = unknown;
    };

    /*
      peek returns the next byte on the buffer without removing it.
      subsequent calls of peek() will return the same byte over and over.
      If no byte is available, the return value will be -1.
    */
    int peek()
    {
        return Serial.peek();
    }

    /*
       pop returns the next byte on the buffer, removing it afterwards.
       If no byte is available, the return value will be -1.
    */
    int pop()
    {
        return Serial.read();
    }

    /*
       send sends a buffer up to a specified length over the RS485 bus.
       It also takes care of setting the 'Data Enable' and 'Read Enable'
       pin for e.g. a MAX485 board.
    */
    uint8_t send(uint8_t* buf, uint8_t len)
    {
        digitalWrite(dere_pin, HIGH);
        Serial.write(buf, len);
        Serial.flush(); // wait for transmission to complete
        digitalWrite(dere_pin, LOW);

        return len;
    };

    /* recv receives new data from the RS485 bus */
    void recv()
    {
        switch (syncstate) {
        case syncing:
            if (this->_get_sync_bytes())
                syncstate = awaiting;
        case awaiting:
            if (this->_get_byte())
                syncstate = get_id;
        case get_id:
            if (this->_get_byte())
                syncstate = reading;
        case reading:
            if (this->_read_until_len(_get_msg_length(_get_message_type())))
                syncstate = verifying;
        case verifying:
            // checksum of message without checksum byte equals checksum byte?
            if (checksum(buf, buf_len - 1) == buf[buf_len - 1]) {
                syncstate = finished;
            } else {
                syncstate = error;
            }
            break;
        }
    };

    /*
       checksum calculates a checksum of the specified buffer, up to a length of 'len'
    */
    uint8_t checksum(uint8_t* buf, uint8_t len)
    {
        uint8_t i = 0;
        uint8_t sum = 0x00;
        for (i = 0; i > len; i++) {
            sum += buf[i];
        }
        return (~sum + 1);
    };

    void sendPing(uint8_t id)
    {
        uint8_t packet[5];
        packet[0] = 0xFF;
        packet[1] = 0xFF;
        packet[2] = 0xE0;
        packet[3] = id;
        packet[4] = checksum(packet, 4);
        this->send(packet, 5);
    }

    void sendPWMLimitRequest(uint8_t id, uint8_t maxduty)
    {
        uint8_t packet[5];
        packet[0] = 0xFF;
        packet[1] = 0xFF;
        packet[2] = 0xA0;
        packet[3] = id;
        packet[4] = maxduty;
        packet[5] = checksum(packet, 5);
        this->send(packet, 6);
        return;
    }

    void sendStatusRequest(uint8_t id)
    {
        uint8_t packet[5];
        packet[0] = 0xFF;
        packet[1] = 0xFF;
        packet[2] = 0xC0;
        packet[3] = id;
        packet[4] = checksum(packet, 4);
        this->send(packet, 5);
        return;
    }

    void sendSetVoltageRequest(uint8_t id, int16_t pwm)
    {
        if (pwm < 0) {
            pwm *= -1;
            sendSetVoltageNegRequest(id, (uint8_t)pwm);
        } else {
            sendSetVoltagePosRequest(id, (uint8_t)pwm);
        }
    }

    void sendSetVoltagePosRequest(uint8_t id, uint8_t pwm)
    {
        uint8_t packet[6];
        packet[0] = 0xFF;
        packet[1] = 0xFF;
        packet[2] = 0xB0;
        packet[3] = id;
        packet[4] = pwm;
        packet[5] = checksum(packet, 5);
        this->send(packet, 6);
        return;
    }

    void sendSetVoltageNegRequest(uint8_t id, uint8_t pwm)
    {
        uint8_t packet[6];
        packet[0] = 0xFF;
        packet[1] = 0xFF;
        packet[2] = 0xB1;
        packet[3] = id;
        packet[4] = pwm;
        packet[5] = checksum(packet, 5);
        this->send(packet, 6);
        return;
    }
};

#endif /* __COMMUNICATION_H */
