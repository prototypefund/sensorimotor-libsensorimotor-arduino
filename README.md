# Sensorimotor-arduino

Sensorimotor-arduino is the arduino-library to control the sensorimotor boards.

## Initialization:

You can initialize a new Motorcord class like this:

```cpp
#include <sensorimotor.h>

// instantiate the motor cord
Motorcord motors;

void setup() {
	// find boards and configure them
	motors.init();
}

void loop() {
	// perform communication and control
	motors.apply(); // should be run at least twice per millisecond
}
```

this will automatically query for motors on the bus.

now you can use the motors class to access all connected motors:

```cpp
motors.get(id)
```

