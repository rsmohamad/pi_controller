#ifndef DS18B20_H
#define DS18B20_H

#include <fstream>
#include <string>

class DS18B20 {
 public:
  DS18B20();
  ~DS18B20() { tempInput.close(); }
  double readTemperature();
  bool isAvailable();

 private:
  std::ifstream tempInput;
  std::string devName;
};

#endif  // DS18B20_H
