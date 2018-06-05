#ifndef PINDEFS_H
#define PINDEFS_H

#include <string>
#include <sstream>
#include <vector>

enum
{
  HEATER_PW = 0,
  HEATER_PUMP,
  HEATER_VALVE_OPEN,
  HEATER_VALVE_CLOSE,
  HEATER_TEMP,
  ATOM_PW,
  ATOM_PUMP,
  ATOM_VALVE_OPEN,
  ATOM_VALVE_CLOSE,
  NANO_PUMP,
  FILTER_VALVE_OPEN,
  FILTER_VALVE_CLOSE,
  MASTER_SW,
  NANO_PUMP_EN,
  NUM_PINS
};

static const unsigned int gpioNum[NUM_PINS] = {
  21, 12, 16, 20, 4, 19, 13, 5, 6, 18, 17, 27, 22, 23
};

static const std::string STATE_NAMES[12] = {
    "HEATER_PW",
    "HEATER_PUMP",
    "HEATER_VALVE_OPEN",
    "HEATER_VALVE_CLOSE",
    "HEATER_TEMP",
    "ATOM_PW",
    "ATOM_PUMP",
    "ATOM_VALVE_OPEN",
    "ATOM_VALVE_CLOSE",
    "NANO_PUMP",
    "FILTER_VALVE_OPEN",
    "FILTER_VALVE_CLOSE"
};

static std::vector<std::string> split(std::string line, char delim){
    std::string buf;
    std::istringstream ss(line);
    std::vector<std::string> tokens;

    while(std::getline(ss, buf, delim))
        tokens.push_back(buf);
    if (buf == "\0")
        tokens.push_back(buf);

    return tokens;
}

#endif // PINDEFS_H
