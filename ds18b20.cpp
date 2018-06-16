#include "ds18b20.h"
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <QTimer>

const std::string prefixPath = "/sys/bus/w1/devices/";
const std::string suffixPath = "/w1_slave";

DS18B20::DS18B20() {
  // Load kernel modules
  int r1 = system("modprobe w1-gpio");
  int r2 = system("modprobe w1-therm");
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
  path = prefixPath + devName + suffixPath;
  std::cout << path << std::endl;
  // tempInput.open(path.c_str());
}

DS18B20::~DS18B20() { tempInput.close(); }

void DS18B20::readTemperature() {
  tempInput.open(path.c_str());
  if (!tempInput.is_open()) {
    return;
  }

  double temperature = -1;
  std::string line;
  std::getline(tempInput, line);

  if (line.find("YES") != std::string::npos) {
    std::getline(tempInput, line);
    int pos = line.find("t=") + 2;
    temperature = std::stod(line.substr(pos));
    temperature /= (double)1000.0;
  }

  tempInput.close();
  emit updateTemperature(temperature);
}

bool DS18B20::isAvailable() {
  tempInput.open(path.c_str());
  bool isOpen = tempInput.is_open();
  tempInput.close();
  return isOpen;
}

void DS18B20::run() {
  QTimer timer;
  connect(&timer, SIGNAL(timeout()), this, SLOT(readTemperature()),
          Qt::DirectConnection);

  timer.setInterval(100);
  timer.start();
  exec();

  timer.stop();
}
