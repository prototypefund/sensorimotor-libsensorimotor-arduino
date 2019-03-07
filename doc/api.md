# API
The sensorimotor API differs a little bit from platform to platform due to capabilities, but for the reference implementation it looks like this:

## `Motorcord`
Motorcord is the main class, which corresponds to a RS485 bus with multiple sensorimotors attached to it.

## `Board`
A Board corresponds to a single sensorimotor board, it has a ID for reference, and can set different attributes like for example

* Voltage limits
* PID and CSL parameters
* Current Position
* observed angular velocity
* observed Voltage
* observed current 
* measured temperature
