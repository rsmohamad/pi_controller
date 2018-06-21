#include "sequence.h"
#include "pindefs.h"

#include <fstream>
#include <iostream>

Sequence::Sequence(const std::string &filename) {
  readCSV(filename);
  std::cout << totalTime << std::endl;
}

void Sequence::readCSV(const std::string &filename) {
  std::ifstream input(filename);
  if (!input.is_open()) return;

  std::string line;
  std::getline(input, line);
  std::vector<std::string> tokens = split(line, ',');
  totalTime = std::atoi(tokens[tokens.size() - 1].c_str());
  state.resize(12);

  while (std::getline(input, line)) {
    tokens = split(line, ',');
    std::vector<int> seq;
    std::string name = *tokens.begin();

    for (auto it = tokens.begin() + 1; it != tokens.end(); ++it) {
      int val = std::atoi(it->c_str());
      seq.push_back(val);
    }

    for (int i = 0; i < MASTER_SW; i++)
      if (name == STATE_NAMES[i]) {
        state[i] = seq;
        break;
      }
  }

  input.close();
}

void Sequence::applyState(NanoCoater *nanoCoater, int timeStep) {
  if (nanoCoater == nullptr) return;

  if (!isRunning) start(nanoCoater);

  for (int i = 0; i < MASTER_SW; i++) {
    switch (i) {
      case HEATER_TEMP:
        break;
      case ATOM_PUMP:
        nanoCoater->setAtomPumpSpeed(state[ATOM_PUMP][timeStep]);
        break;
      case HEATER_PUMP:
        nanoCoater->setHeaterPumpSpeed(state[HEATER_PUMP][timeStep]);
        break;
      case NANO_PUMP:
        nanoCoater->setStepperFrequency(state[NANO_PUMP][timeStep]);
        break;
      default:
        nanoCoater->setGPIOState(i, state[i][timeStep]);
        break;
    }

    // std::cout << state[i][timeStep] << std::endl;
  }
}

// Apply temperature control according to what client wants
void Sequence::applyState(NanoCoater *nanoCoater, int timeStep,
                          double temperature) {
  std::cout << state[ATOM_PUMP][timeStep] << " ";
  std::cout << state[HEATER_PUMP][timeStep] << " ";
  std::cout << state[NANO_PUMP][timeStep] << std::endl;
  if (nanoCoater == nullptr) return;
  applyState(nanoCoater, timeStep);

  double targetTemperature = state[HEATER_TEMP][timeStep];

  // Do nothing if target temperature outside range
  if (targetTemperature < 40 || targetTemperature > 100) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
    return;
  }

  // Turn on heater if temperature below target temperature
  if (temperature < targetTemperature) {
    nanoCoater->setGPIOState(HEATER_PW, 1);
    heaterOn = true;
  }

  // Turn off heater if temperature 5 above target and was heating
  if (temperature >= (targetTemperature + 5) && heaterOn) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
  }

  // Default behavior if temperature is above target
  if (temperature >= targetTemperature && !heaterOn) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
  }
}

void Sequence::start(NanoCoater *nanoCoater) {
  if (nanoCoater == nullptr) return;

  isRunning = true;
  nanoCoater->setGPIOState(MASTER_SW, 1);
}

void Sequence::stop(NanoCoater *nanoCoater) {
  if (nanoCoater == nullptr) return;

  isRunning = false;
  nanoCoater->setGPIOState(MASTER_SW, 0);
}
