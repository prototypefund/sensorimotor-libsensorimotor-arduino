#!/bin/bash

baud=4000
avr=atmega328p
dst=$avr.`date +"%Y%m%d_%H%M%S"`
port=/dev/ttyACM0
programmer=stk500v2

# calibration  One or more bytes of RC oscillator calibration data.
# eeprom       The EEPROM of the device.
# efuse        The extended fuse byte.
# flash        The flash ROM of the device.
# fuse         The fuse byte in devices that have only a single fuse byte.
# hfuse        The high fuse byte.
# lfuse        The low fuse byte.
# lock         The lock byte.
# signature    The three device signature bytes (device ID).
# fuseN        The fuse bytes of ATxmega devices, N is an integer number for each fuse supported by the device.
# application  The application flash area of ATxmega devices.
# apptable     The application table flash area of ATxmega devices.
# boot         The boot flash area of ATxmega devices.
# prodsig      The production signature (calibration) area of ATxmega devices.
# usersig      The user signature area of ATxmega devices.

for memory in calibration eeprom efuse flash fuse hfuse lfuse lock signature application apptable boot prodsig usersig; do
	avrdude -p $avr -c $programmer -P $port -b $baud -U $memory:r:/dev/stdout:i > ./$dst.$memory.hex ||
	rm ./$dst.$memory.hex
done
