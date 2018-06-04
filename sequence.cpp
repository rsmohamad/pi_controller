#include "sequence.h"
#include "pindefs.h"

#include <fstream>
#include <sstream>
#include <iostream>

const std::string STATE_NAMES[12] = {
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

std::vector<std::string> split(std::string line, char delim){
    std::string buf;
    std::istringstream ss(line);
    std::vector<std::string> tokens;

    while(std::getline(ss, buf, delim))
        tokens.push_back(buf);
    if (buf == "\0")
        tokens.push_back(buf);

    return tokens;
}

Sequence::Sequence(const std::string &filename) {
    readCSV(filename);
    std::cout << totalTime << std::endl;
}

void Sequence::readCSV(const std::string &filename){
    std::ifstream input(filename);
    if (!input.is_open())
        return;

    std::string line;
    std::getline(input, line);
    std::vector<std::string> tokens = split(line, ',');
    totalTime = std::atoi(tokens[tokens.size()-1].c_str());
    state.resize(12);

    while(std::getline(input, line)){
        tokens = split(line, ',');
        std::vector<int> seq;
        std::string name = *tokens.begin();

        for (auto it = tokens.begin()+1; it != tokens.end(); ++it){
            int val = std::atoi(it->c_str());
            seq.push_back(val);
        }

        for(int i = 0; i < 12; i++)
            if (name == STATE_NAMES[i]){
                state[i] = seq;
                break;
            }
    }

    input.close();
}

void Sequence::applyState(NanoCoater *nanoCoater, int timeStep){
    if (nanoCoater == nullptr)
        return;

    if (!isRunning)
        start(nanoCoater);

    for (int i = 0; i < 12; i++){
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
        default:
            nanoCoater->setGPIOState(i, state[i][timeStep]);
            break;
        }
    }
}

void Sequence::start(NanoCoater *nanoCoater){
    if (nanoCoater == nullptr)
        return;

    isRunning = true;
    nanoCoater->setGPIOState(gpioNum[MASTER_SW], 1);
}

void Sequence::stop(NanoCoater *nanoCoater){
    if (nanoCoater == nullptr)
        return;

    isRunning = false;
    nanoCoater->setGPIOState(gpioNum[MASTER_SW], 0);
}
