
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
  DS18B20 tempSensor;
  if (tempSensor.isAvailable()) {
    QObject::connect(&tempSensor, SIGNAL(updateTemperature(double)), &root,
                     SLOT(onTemperatureUpdate(double)), Qt::QueuedConnection);
    tempSensor.start();
  } else {
    QMessageBox msgBox;
    msgBox.setText("Temperature sensor is not available");
    msgBox.exec();
  }

  if (gpioInitialise() >= 0)
    root.setNanoCoater(new NanoCoater());
  else {
    QMessageBox msgBox;
    msgBox.setText("Raspberry Pi GPIO not supported");
    msgBox.exec();
  }

  window.setCentralWidget(&root);
  window.resize(1024, 600);
  window.setWindowTitle("Pi Controller");
  window.show();

  return a.exec();

  tempSensor.quit();
  tempSensor.wait();
}
