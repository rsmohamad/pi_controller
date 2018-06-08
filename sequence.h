#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "nanocoater.h"
#include "pindefs.h"

#include <QPoint>
#include <vector>

class Sequence {
 public:
  Sequence(const std::string &filename);
  void stop(NanoCoater *);
  void applyState(NanoCoater *, int timeStep);
  void applyState(NanoCoater *, int timeStep, double temperature);
  std::vector<std::vector<int>> state;
  int totalTime = 0;

 private:
  void readCSV(const std::string &filename);
  void start(NanoCoater *);
  bool isRunning = false;
  bool heaterOn = false;
};

#endif  // SEQUENCE_H
