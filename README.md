# Sensorimotor-arduino [![Build Status](https://travis-ci.org/sensorimotor/libsensorimotor-arduino.svg?branch=master)](https://travis-ci.org/sensorimotor/libsensorimotor-arduino)

![Breadboard example](doc/images/breadboard_minimal.png)

Sensorimotor-arduino is the arduino-library to control the sensorimotor boards from an embedded platform, like arduino uno, arduino nano or compatible boards such as STM32 or ESP32.

## Documentation

For the full documentation, see the [documentation folder](doc/README.md) and the [example projects](example/README.md).

## Quick start:

### Installation

Clone this repository as a new folder called "Sensorimotor" under the folder named "libraries" in your Arduino sketchbook folder. Create the folder "libraries" in case it does not exist yet.

### Usage

To use the library in your own sketch, select it from Sketch > Import Library.

This will add all referenced Header files, but you may delete all `#include`s, except the line

```cpp
#include <Sensorimotor.h>
```

You can now initialize a new Motorcord class like this:

```cpp
#include <ensorimotor.h>

// instantiate the motor cord
Motorcord motors;

void setup() {
	// find boards and configure them
	motors.begin();
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
