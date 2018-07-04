#ifndef MANUAL_H
#define MANUAL_H

#include <string>
#include <vector>
#include "nanocoater.h"

class Manual {
 public:
  Manual();
  int toggleState(int);
  void tempControl(int);
  void setCoater(NanoCoater *);
  void clearState();
  std::string getLabel(int);

 private:
  NanoCoater *nanoCoater = nullptr;
  std::vector<int> state;
  void applyState();
  bool heaterOn = false;
};

#endif  // MANUAL_H
