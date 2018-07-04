#include "manual.h"

const int MANUAL_PWM = 20;
const int MANUAL_TEMP = 40;
const int MANUAL_STEPPER = 800;

Manual::Manual() { clearState(); }

int Manual::toggleState(int num) {
  state[num] = !state[num];
  switch (num) {
    case HEATER_TEMP:
      state[num] *= MANUAL_TEMP;
      break;
    case ATOM_PUMP:
      state[num] *= MANUAL_PWM;
      break;
    case HEATER_PUMP:
      state[num] *= MANUAL_PWM;
      break;
    case NANO_PUMP:
      state[num] *= MANUAL_STEPPER;
      break;
    default:
      break;
  }

  applyState();
  return state[num];
}

void Manual::tempControl(int temperature) {
  if (nanoCoater == nullptr) return;

  if (state[HEATER_TEMP] < 40 || state[HEATER_TEMP] > 100) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
    return;
  }

  // Turn on heater if temperature below target temperature
  if (temperature < MANUAL_TEMP) {
    nanoCoater->setGPIOState(HEATER_PW, 1);
    heaterOn = true;
  }

  // Turn off heater if temperature 5 above target and was heating
  if (temperature >= (MANUAL_TEMP + 5) && heaterOn) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
  }

  // Default behavior if temperature is above target
  if (temperature >= MANUAL_TEMP && !heaterOn) {
    nanoCoater->setGPIOState(HEATER_PW, 0);
    heaterOn = false;
  }

  std::cout << temperature << std::endl;
}

void Manual::setCoater(NanoCoater* coater) { this->nanoCoater = coater; }

void Manual::clearState() {
  state.clear();
  for (int i = 0; i < MASTER_SW; i++) state.push_back(0);
}

std::string Manual::getLabel(int num) {
  if (num == HEATER_TEMP || num == ATOM_PUMP || num == HEATER_PUMP ||
      num == NANO_PUMP)
    return std::to_string(state[num]);

  return state[num] ? "ON" : "OFF";
}

void Manual::applyState() {
  if (nanoCoater == nullptr) return;
  for (int i = 0; i < MASTER_SW; i++) {
    switch (i) {
      case HEATER_TEMP:
        break;
      case ATOM_PUMP:
        nanoCoater->setAtomPumpSpeed(state[ATOM_PUMP]);
        break;
      case HEATER_PUMP:
        nanoCoater->setHeaterPumpSpeed(state[HEATER_PUMP]);
        break;
      case NANO_PUMP:
        nanoCoater->setStepperFrequency(state[NANO_PUMP]);
        break;
      default:
        nanoCoater->setGPIOState(i, state[i]);
        break;
    }
  }
}
