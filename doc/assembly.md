## Assembling the board

After you aquired a base board with all SMD components pre-soldered, some final assembly is required. For this you'll need:

- a 220μF Capacitor
- 2x 4-Pin Molex Spox mini plugs
- a 6x2 pin dupont ISP-Header
- a 2x4 pin I2C header
- a thermistor (optional)

Solder the capacitor into the designated holes from the backside (watch the correct polarity!)

Solder the Spox mini plugs on the designated ports labeled 'RS485'.
Solder the I2C and ISP headers in the correct areas, labeled I2C and ISP respectively.
The thermistor can be soldered last, the recommended placement is between the H-Bridge and the motor, so a temperature rise there can be measured.

After the preparation, the servo body with the motor and potentiometer can be attached: simply solder the potentiometer and the motor to the designated pin pads.
The polarity is important here, as this will determine the direction in which the motor will turn, and the direction that the potentiometer will measure

## Flashing the firmware

On a new board, the microcontroller is not running any instructions yet. To upload firmware, you will need a Programmer and the firmware `.hex` file. Upload the hex file using the supplied script with `make flash`
