#ifndef __BUFFER_H
#define __BUFFER_H

/*
   WIP
*/
class buf {
public:
    uint8_t buf[20];
    uint8_t len = 0;
    // number of read bytes
    uint8_t read_bytes = 0;
    void append(uint8_t byte)
    {
        buf[len] = byte;
        len++;
    }
};

#endif /* __BUFFER_H */
