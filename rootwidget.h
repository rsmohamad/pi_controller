#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QChartView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCore/QTimer>

#include "chart.h"
#include "ds18b20.h"
#include "nanocoater.h"
#include "sequence.h"

class RootWidget : public QWidget {
  Q_OBJECT
 public:
  explicit RootWidget(QWidget *parent = nullptr);
  void setNanoCoater(NanoCoater *);
  void setTempSensor(DS18B20 *);

 signals:

 public slots:
  void handleExit();
  void handleCSV();
  void handleStart();
  void handleStop();
  void handleTimer();
  void handleTempRead();

 private:
  QWidget *buttonPane;
  QChartView *chartView;

  QHBoxLayout *hbox;
  QVBoxLayout *vbox;

  QPushButton *start;
  QPushButton *exit;
  QPushButton *load;
  QPushButton *stop;
  QLabel *tempReading;

  Chart *chart = nullptr;
  Sequence *seq = nullptr;
  QTimer m_timer;
  QTimer tempTimer;

  int timeStep = 0;
  NanoCoater *coater = nullptr;
  DS18B20 *tempSensor = nullptr;
  double temperature = 0.0;
};

#endif  // MAINWINDOW_H
