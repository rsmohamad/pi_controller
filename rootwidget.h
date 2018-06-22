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
  void paintEvent(QPaintEvent *event);
  void setStatusButtons(int);

 signals:

 public slots:
  void handleExit();
  void handleCSV();
  void handleStart();
  void handleStop();
  void handleTimer();
  void handleManual();
  void handleStatusButton(int);
  void onTemperatureUpdate(double);

 private:
  QWidget *rightPane, *leftPane;
  QChartView *chartView;

  QHBoxLayout *hbox;
  QVBoxLayout *rightVBox, *leftVBox;

  QPushButton *start;
  QPushButton *exit;
  QPushButton *load;
  QPushButton *stop;
  QPushButton *manual;
  QLabel *tempReading;
  QTimer m_timer;

  std::vector<QPushButton *> statusButtons;

  Chart *chart = nullptr;
  Sequence *seq = nullptr;
  NanoCoater *coater = nullptr;

  double temperature = 0;
  int timeStep = 0;
  bool isManual = false;
};

#endif  // MAINWINDOW_H
