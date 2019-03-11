#include <stdint.h>

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

const uint8_t MAX_MSG_LEN = 20;

enum command_t : uint8_t {
    CMD_UNKNOWN = 0x00,
    // requests
    CMD_REQUEST_PWM_LIMIT = 0xA0,
    CMD_REQUEST_DATA = 0xC0,
    CMD_REQUEST_LED_TOGGLE = 0xD0,
    CMD_REQUEST_PING = 0xE0,
    CMD_REQUEST_POS_PWM = 0xB0,
    CMD_REQUEST_NEG_PWM = 0xB1,
    // responses
    CMD_RESPONSE_DATA = 0x80,
    CMD_RESPONSE_PONG = 0xE1,
};

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

class Communication {
private:
    /*
     * `dere_pin` is the number of the pin connected to the
     * Data Enable/Read Enable pin of the RS485 transceiver module
     */
    uint8_t dere_pin; // data enable/read enable pin number

    // the receive buffer
    uint8_t buf[MAX_MSG_LEN];
    uint8_t _buf_len; // number of utilized bytes
    // _read_bytes keeps track of bytes read from the receive buffer
    uint8_t _read_bytes = 0;

    /*
     * `comtype` is the detected command type of the current frame.
     * do not read this value directly, use `_get_message_type`
     * instead.
     */
    command_t comtype = CMD_UNKNOWN;

    /*
     * `_peek` returns the next byte on the serial buffer without removing it.
     * subsequent calls of peek() will return the same byte over and over.
     * If no byte is available, the return value will be -1.
     */
    int _peek()
    {
        return Serial.peek();
    }

    /*
     * `_pop` returns the next byte on the serial buffer, removing it afterwards.
     * If no byte is available, the return value will be -1.
     */
    int _pop()
    {
        return Serial.read();
    }

    /*
     * `_get_sync_bytes` eats bytes until two successive sync bytes are encountered,
     * after which it will stop eating and returns `true`.
     * if the serial buffer does not contain two successive sync bytes, `false` will
     * be returned.
     */
    bool _get_sync_bytes()
    {
        uint8_t sync = 0;
        for (;;) {
            // once we have two sync bytes, we are successful
            if (sync == 2) {
                // add sync bytes to buffer
                buf[_buf_len] = 0xFF;
                _buf_len++;
                buf[_buf_len] = 0xFF;
                _buf_len++;
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
     * `_get_byte()` works like `_pop()`, but adds the read byte automatically
     * to the received buffer.
     */
    bool _get_byte()
    {
        int received = _pop();
        if (received < 0) {
            return false;
        }

        // add byte to buffer
        buf[_buf_len] = (uint8_t)received;
        _buf_len++;
        return true;
    }

    /*
     * `_get_message_type` returns the command ID of the currently
     * processed frame, or 0x00 if the command ID could not be
     * determined (i.e not enough bytes are received yet).
     */
    command_t _get_message_type()
    {
        if (_buf_len < 3)
            return (command_t)0x00;

        return (command_t)buf[2];
    };

    /*
     * `_get_message_length` returns the expected message length for a specific message type in bytes,
     * including the sync bytes and chcecksum.
     */
    int8_t _get_msg_length(uint8_t msg_id)
    {
        switch (msg_id) {
        case CMD_RESPONSE_DATA:
            return 15;
        case CMD_RESPONSE_PONG:
            return 5;
        default:
            return -1; // error
        }
    }

    /*
     * `_read_until_len` reads more bytes from the serial input, until
     * the desired message length is reached.
     */
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

            buf[_buf_len] = (uint8_t)received;
            _buf_len++;

            if (_buf_len == msg_len)
                // we reached required amount of bytes
                return true;
        }
    }

public:
    /*
     * `syncstate` is the current state of the RX communication for this
     * timeslot. if the syncstate is
     * `Communication::command_state_t::finished`, the frame in the buffer
     * is correctly received, validated, and can be parsed.
     */
    command_state_t syncstate = syncing;

    // default constructor
    Communication()
        : dere_pin(10)
    {
        pinMode(dere_pin, OUTPUT);
    };

    // constructor, overloaded to specify a non-default DE/RE pin
    Communication(uint8_t dere_pin)
        : dere_pin(dere_pin)
    {
        pinMode(dere_pin, OUTPUT);
    };
    ~Communication(){};

    /*
     * `reset` will delete the already read frame data and reset
     * the communication state, for example when a timeslot has
     * ended
     */
    void reset()
    {
        syncstate = syncing;
        _buf_len = 0;
        _read_bytes = 0;
        comtype = CMD_UNKNOWN;
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
     * `get_message_type` returns the command ID of the currently
     * processed frame, or 0x00 if the command ID could not be
     * determined (i.e not enough bytes are received yet).
     */
    command_t get_message_type()
    {
        return get_message_type();
    }

    /*
     * send sends a buffer up to a specified length over the RS485 bus,
     * automatically prepending the sync bytes and appending the checksum.
     * It also takes care of setting the 'Data Enable' and 'Read Enable'
     * pin for a transceiver board board.
     */
    uint8_t send(uint8_t* buf, uint8_t len)
    {
        // sync bytes
        uint8_t sendbuf[MAX_MSG_LEN] = { 0xFF, 0xFF };
        uint8_t sendbuf_len = 2;

        // append buf
        memcpy(sendbuf + sendbuf_len, buf, len);
        sendbuf_len += len;

        // append checksum
        sendbuf[sendbuf_len] = checksum(sendbuf, sendbuf_len);
        sendbuf_len++;

        digitalWrite(dere_pin, HIGH);
        Serial.write(sendbuf, len);
        Serial.flush(); // wait for transmission to complete
        digitalWrite(dere_pin, LOW);

        return len;
    };

    /*
     * recv receives new data from the RS485 bus into the buffer. It will
     * append new data on subsequent calls and update the sync state.
     */
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
            if (checksum(buf, _buf_len - 1) == buf[_buf_len - 1]) {
                syncstate = finished;
            } else {
                syncstate = error;
            }
            break;
        }
    };

    /*
     * checksum calculates a checksum of the specified buffer, up to a length of 'len'
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

    void send_ping(uint8_t id)
    {
        uint8_t packet[2] = { CMD_REQUEST_PING, id };
        this->send(packet, 2);
    }

    void send_pwm_limit_request(uint8_t id, uint8_t maxduty)
    {
        uint8_t packet[3] = {
            CMD_REQUEST_PWM_LIMIT,
            id,
            maxduty
        };
        this->send(packet, 3);
        return;
    }

    void send_data_request(uint8_t id)
    {
        uint8_t packet[2] = {
            CMD_REQUEST_DATA,
            id
        };
        this->send(packet, 2);
        return;
    }

    void send_set_voltage_request(uint8_t id, int16_t pwm)
    {
        if (pwm < 0) {
            pwm *= -1;
            if (pwm > 255)
                pwm = 255;
            send_set_voltage_neg_request(id, (uint8_t)pwm);
        } else {
            if (pwm > 255)
                pwm = 255;
            send_set_voltage_pos_request(id, (uint8_t)pwm);
        }
    }

    void send_set_voltage_pos_request(uint8_t id, uint8_t pwm)
    {
        uint8_t packet[3] = {
            packet[0] = CMD_REQUEST_POS_PWM,
            packet[1] = id,
            packet[2] = pwm
        };
        this->send(packet, 3);
        return;
    }

    void send_set_voltage_neg_request(uint8_t id, uint8_t pwm)
    {
        uint8_t packet[3] = {
            CMD_REQUEST_NEG_PWM,
            id,
            pwm
        };
        this->send(packet, 3);
        return;
    }
};

#endif /* __COMMUNICATION_H */
