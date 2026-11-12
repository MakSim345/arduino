#ifndef EDU67_CONFIG_H
#define EDU67_CONFIG_H

/*
 * Choose which I2C bus driver the sketch uses.
 * Both implement the same I2cDrvIf, so the sensor drivers do not change at all.
 */

/*** Define ONLY ONE of it: ***/
#define I2C_BACKEND_WIRE     // Lesson 2: blocking driver built on top of the Wire library
// #define I2C_BACKEND_IT    // Lesson 3: our own interrupt-driven TWI driver (no Wire library)
/******************************/

#if defined(I2C_BACKEND_WIRE) == defined(I2C_BACKEND_IT)
#error "Config.h: define exactly one of I2C_BACKEND_WIRE / I2C_BACKEND_IT"
#endif

#if defined(I2C_BACKEND_IT) && defined(ARDUINO) && !defined(ARDUINO_ARCH_AVR)
#error "I2C_BACKEND_IT talks to AVR TWI registers directly: use an AVR board (Uno, Nano, Mega)"
#endif

#endif // EDU67_CONFIG_H
