Protocol

Message types

## 0xA0 Set PWM Limit Request

`FF FF A0 00 40 22`
(no response)

## 0xC0 Data request

`FF FF C0 00 42`
will result in a 0x80 Data response.

## 0xD0 Toggle LED

`FF FF C0 00 XX`
(no response)

## 0x80 Data response

`FF FF 80 00 01 66 00 09 00 20 03 C4 FF FF 2D`
the data is structured as follows

- data[4:5] Position (0-1023)
- data[6:7] Current ( / 0.003225806f [max 1023 -> 3A3])
- data[8:9] Velocity
- data[10:11] Voltage 12.25V ( / 0.012713472f [max 1023-> 13V])
- data[12:13] Temperature (0xFFFF if no thermistor is connected)

The temperature data is calculated by the formula
`((55*WORD)-25450)*0.01` where word is [0,1023] or 0xFF on error.
Therefore temperature ranges of ~-250°C to ~+300°C cold be mapped,
but probably your measured values should be expected to be 0-100°C.

## 0xE0 Ping

`FF FF E0 00 XX`
will result in a 0xE1 Pong response

## 0xE1 Pong

`FF FF E1 00 XX`

# 0xB0 Set positive voltage

`FF FF B0 00 01 XX`
board will respond with 0x80 Data response.

## 0xB1 Set negative voltage

`FF FF B1 00 01 XX`
board will respond with 0x80 Data response.

# 0xA0 Set voltage limit

`FF FF A0 00 40 XX`
no response

will limit the maximum PWM, thus effectively limit the voltage.
In this example PWM is limited to 64 (0x40), one quarter of the maximum duty
cycle of 255, effectively limiting the voltage to a quarter of the input
voltage.

# 0x40 External Sensor request

`FF FF 40 00 XX`

will request data reported by the the connected I2C sensor.
