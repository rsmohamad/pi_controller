#ifndef GPIO_H
#define GPIO_H

#include <pigpio.h>
#include "pindefs.h"

#include <iostream>

class NanoCoater {
 public:
  NanoCoater() {
    for (int i = 0; i < NUM_PINS; i++)
      if (i != HEATER_TEMP) gpioSetMode(gpioNum[i], PI_OUTPUT);

    gpioSetMode(gpioNum[NANO_PUMP], PI_ALT5);
    gpioSetPWMfrequency(gpioNum[HEATER_PUMP], 200);
    gpioSetPWMfrequency(gpioNum[ATOM_PUMP], 200);

    setDefaultState();
  }

  void setGPIOState(unsigned int pin, int state) {
    if (state != 1 && state != 0) state = 0;
    gpioWrite(gpioNum[pin], state);
    // std::cout << gpioNum[pin] << std::endl;
  }

  void setAtomPumpSpeed(int speed) { gpioPWM(gpioNum[ATOM_PUMP], speed); }

  void setHeaterPumpSpeed(int speed) { gpioPWM(gpioNum[HEATER_PUMP], speed); }

  void setStepperFrequency(int freq) {
    if (freq <= 0) {
      setGPIOState(NANO_PUMP_EN, 1);
      gpioHardwarePWM(gpioNum[NANO_PUMP], 0, 0);
    } else {
      setGPIOState(NANO_PUMP_EN, 0);
      gpioHardwarePWM(gpioNum[NANO_PUMP], 100, freq);
    }
  }

  void setDefaultState() {
    for (unsigned int i = 0; i < NUM_PINS; i++) {
      switch (i) {
        case HEATER_TEMP:
          break;
        case ATOM_PUMP:
          setAtomPumpSpeed(0);
          break;
        case HEATER_PUMP:
          setHeaterPumpSpeed(0);
          break;
        case NANO_PUMP:
          setStepperFrequency(0);
          break;
        case NANO_PUMP_EN:
          setGPIOState(NANO_PUMP_EN, 1);
          break;
        default:
          setGPIOState(i, 0);
          break;
      }
    }
  }

 private:
};

#endif  // GPIO_H
