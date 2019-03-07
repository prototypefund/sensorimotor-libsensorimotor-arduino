# Bus

why use a bus?

The sensorimotor platform uses a bus system for controlling the individual servos. Practically speaking, this means multiple boards are connected to the same data lines.
This has some advantages and drawbacks that you should be aware of.
The sensorimotor uses a RS485 bus, which is a common standard. However only the electrical layer is standardized, thus the binary protocol used by the sensorimotor will not automatically be understood by all RS485-compatible devices.

The RS485 bus consists of two data wires: A (which is sometimes called D-) and B (which is sometimes called D+). To allow for reliable communication, this data cables should be twisted and all receivers should share a common ground. Additionally, termination of the bus is required starting from a length of more than a couple centimeters. This can be done by using one 220Ω to connect the A and B data lines on either end of the bus.

###  Prerequisites 

* RS485 needs common ground
* RS485 needs end termination
* RS485 benefits from twisted pairs
* All required wiring logic is already included on the max485 transceiver board (arduino), or rs485 usb stick (windows/mac/linux)

Sensorimotor's cable layout is as follows (from left to right):

* B (sometimes called D+)  } Twisted
* A (sometimes called D-)   }
* 12V DC (usually colored cable)
* GND

Recommended for embedded chips: MAX485 or SN75176.
The wiring for this setup is as follows:

* B on the transceiver is connected to B on the Sensorimotor
* A on the transceiver is connected to A on the Sensorimotor
* Both the receiver and Sensorimotor share a common ground (this is most easily achieved by directly connecting their GND Pins)
* RO on the Transceiver should go to the RX Pin on the Arduino
* DI on the Transceiver should to go the TX Pin on the Arduino
* DE **and** RE on the receiver should go to to a shared digital output capable Pin

DE and RE control the sending and receiving mode, if DE and RE are High, the Transceiver will send all signals arriving on the DI Pin; if DE and RE are low, the transceiver will instead output received signals on RO.
