#ifndef DS18B20_H
#define DS18B20_H

#include <QThread>
#include <fstream>
#include <string>

class DS18B20 : public QThread {
  Q_OBJECT
 signals:
  void updateTemperature(double temperature);
 private slots:
  void readTemperature();

 public:
  DS18B20();
  ~DS18B20();
  bool isAvailable();

 private:
  std::ifstream tempInput;
  std::string devName;
  std::string path;
  void run();
};

#endif  // DS18B20_H
