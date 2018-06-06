
#include <QMessageBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>

#include "nanocoater.h"
#include "rootwidget.h"

int main(int argc, char *argv[]) {
  QApplication::setSetuidAllowed(true);
  QApplication a(argc, argv);

  QMainWindow window;
  // window.setWindowFlags(Qt::FramelessWindowHint);

  RootWidget root;
  if (gpioInitialise() >= 0)
    root.setNanoCoater(new NanoCoater());
  else {
    QMessageBox msgBox;
    msgBox.setText("Raspberry Pi GPIO not supported");
    msgBox.exec();
  }

  DS18B20 *tempSensor = new DS18B20();
  if (tempSensor->isAvailable())
    root.setTempSensor(tempSensor);
  else {
    QMessageBox msgBox;
    msgBox.setText("Temperature sensor is not available");
    msgBox.exec();
  }

  window.setCentralWidget(&root);
  window.resize(1024, 640);
  window.setWindowTitle("Pi Controller");
  window.show();

  return a.exec();
}
