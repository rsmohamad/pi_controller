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

    // gpioSetMode(gpioNum[NANO_PUMP], PI_ALT5);
    gpioSetPWMfrequency(gpioNum[HEATER_PUMP], 200);
    gpioSetPWMfrequency(gpioNum[ATOM_PUMP], 200);
    // gpioSetPWMfrequency(gpioNum[NANO_PUMP], 0);

    gpioSetPWMrange(gpioNum[HEATER_PUMP], 100);
    gpioSetPWMrange(gpioNum[ATOM_PUMP], 100);
    // gpioSetPWMrange(gpioNum[NANO_PUMP], 100);

    setDefaultState();

    std::cout << gpioGetPWMrange(gpioNum[HEATER_PUMP]) << "|";
    std::cout << gpioGetPWMrange(gpioNum[ATOM_PUMP]) << std::endl;
  }

  void setGPIOState(unsigned int pin, int state) {
    if (state != 1 && state != 0) state = 0;
    gpioWrite(gpioNum[pin], state);
  }

  void setAtomPumpSpeed(int speed) {
    gpioSetPWMrange(gpioNum[ATOM_PUMP], 100);
    gpioPWM(gpioNum[ATOM_PUMP], speed);
  }

  void setHeaterPumpSpeed(int speed) {
    gpioSetPWMrange(gpioNum[HEATER_PUMP], 100);
    gpioPWM(gpioNum[HEATER_PUMP], speed);
  }

  void setStepperFrequency(int freq) {
    if (freq <= 0) {
      setGPIOState(NANO_PUMP_EN, 1);
      gpioHardwarePWM(gpioNum[NANO_PUMP], 0, 0);
    } else {
      setGPIOState(NANO_PUMP_EN, 0);
      gpioHardwarePWM(gpioNum[NANO_PUMP], freq, 100000);
    }

    std::cout << "Atom pump " << gpioGetPWMdutycycle(gpioNum[ATOM_PUMP]) << "/"
              << gpioGetPWMrange(gpioNum[ATOM_PUMP]) << std::endl;
    std::cout << "Heater pump " << gpioGetPWMdutycycle(gpioNum[HEATER_PUMP])
              << "/" << gpioGetPWMrange(gpioNum[HEATER_PUMP]) << std::endl;
    std::cout << "Nano pump " << gpioGetPWMfrequency(gpioNum[NANO_PUMP])
              << " | " << gpioGetPWMdutycycle(gpioNum[NANO_PUMP]) << "/"
              << gpioGetPWMrange(gpioNum[NANO_PUMP]) << std::endl;
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
