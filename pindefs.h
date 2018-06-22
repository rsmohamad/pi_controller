#ifndef PINDEFS_H
#define PINDEFS_H

#include <sstream>
#include <string>
#include <vector>

enum {
  HEATER_PUMP = 0,
  HEATER_VALVE_OPEN,
  HEATER_VALVE_CLOSE,
  HEATER_TEMP,
  ATOM_PW,
  ATOM_PUMP,
  NANO_PUMP,
  FILTER_VALVE_OPEN,
  FILTER_VALVE_CLOSE,

  // Below this point, pin controls are automated and not user inputted
  MASTER_SW,
  NANO_PUMP_EN,
  HEATER_PW,
  NUM_PINS
};

static const unsigned int gpioNum[NUM_PINS] = {12, 16, 20, 4,  19, 13,
                                               18, 17, 27, 22, 23, 21};

static const std::string STATE_NAMES[MASTER_SW] = {
    "HEATER_PUMP", "HEATER_VALVE_OPEN", "HEATER_VALVE_CLOSE",
    "HEATER_TEMP", "ATOM_PW",           "ATOM_PUMP",
    "NANO_PUMP",   "FILTER_VALVE_OPEN", "FILTER_VALVE_CLOSE"};

#endif  // PINDEFS_H
