#ifndef GPIO_H
#define GPIO_H

#include <pigpio.h>
#include "pindefs.h"

class NanoCoater
{
public:
  NanoCoater()
  {
    for (int i = 0; i < NUM_PINS; i++)
      gpioSetMode(gpioNum[i], PI_OUTPUT);

    gpioSetMode(gpioNum[HEATER_PUMP], PI_ALT0);
    gpioSetMode(gpioNum[ATOM_PUMP], PI_ALT0);
    gpioSetMode(gpioNum[NANO_PUMP], PI_ALT5);

    gpioSetPWMfrequency(gpioNum[HEATER_PUMP], 200);
    gpioSetPWMfrequency(gpioNum[ATOM_PUMP], 200);

  }

  void setGPIOState(int pin, int state){
      if (state != 1 || state != 0)
        state = 0;
      gpioWrite(gpioNum[pin], state);
  }

  void setAtomPumpSpeed(int speed) { gpioPWM(gpioNum[ATOM_PUMP], speed); }

  void setHeaterPumpSpeed(int speed) { gpioPWM(gpioNum[HEATER_PUMP], speed); }

  void setStepperFrequency(int freq)
  {
    if (freq <= 0) {
      setGPIOState(NANO_PUMP_EN, 1);
      gpioHardwarePWM(gpioNum[NANO_PUMP], 0, 0);
      return;
    }
    setGPIOState(NANO_PUMP_EN, 0);
    gpioHardwarePWM(gpioNum[NANO_PUMP], 100, freq);
  }

private:
};

#endif // GPIO_H
