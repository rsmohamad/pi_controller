#include "ds18b20.h"
#include <dirent.h>
#include <string.h>

const std::string prefixPath = "/sys/bus/w1/devices/";
const std::string suffixPath = "/w1_slave";

DS18B20::DS18B20() {
  // Load kernel modules
  system("sudo modprobe w1-gpio");
  system("sudo modprobe w1-therm");
  sleep(1);

  // Get the temperature sensor id
  DIR *d = opendir(prefixPath.c_str());
  dirent *entry;
  while (d && (entry = readdir(d))) {
    if (strncmp(entry->d_name, "28-", 3) == 0) {
      devName = std::string(entry->d_name);
      break;
    }
  }

  // Open file
  std::string path = prefixPath + devName + suffixPath;
  tempInput.open(path.c_str());
}

double DS18B20::readTemperature() {
  if (!tempInput.is_open()) return 0;

  double temperature;
  std::string line;
  std::getline(tempInput, line);

  if (line.find("YES") != std::string::npos) {
    std::getline(tempInput, line);
    int pos = line.find("t=") + 2;
    temperature = std::atof(line.substr(pos).c_str());
    temperature /= 1000.0;
    return temperature;
  }

  return 0;
}

bool DS18B20::isAvailable() { return tempInput.is_open(); }
