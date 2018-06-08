#include "rootwidget.h"
#include "chart.h"

#include <QApplication>
#include <QFileDialog>
#include <QStyle>
#include <QtCharts/QChartView>
#include <iostream>

RootWidget::RootWidget(QWidget *parent)
    : QWidget(parent)

{
  chart = new Chart();
  chart->legend()->hide();
  chartView = new QChartView(chart);
  chartView->setRenderHints(QPainter::Antialiasing);

  load = new QPushButton("Load CSV");
  start = new QPushButton("Start");
  stop = new QPushButton("Stop");
  exit = new QPushButton("Exit");
  tempReading = new QLabel("0.000");
  tempReading->setAlignment(Qt::AlignCenter);
  QFont f("Arial", 16, QFont::Bold);
  tempReading->setFont(f);

  load->setMinimumSize(100, 100);
  start->setMinimumSize(100, 100);
  stop->setMinimumSize(100, 100);
  exit->setMinimumSize(100, 100);

  start->setDisabled(true);
  stop->setDisabled(true);

  QObject::connect(start, SIGNAL(pressed()), this, SLOT(handleStart()));
  QObject::connect(exit, SIGNAL(pressed()), this, SLOT(handleExit()));
  QObject::connect(load, SIGNAL(pressed()), this, SLOT(handleCSV()));
  QObject::connect(stop, SIGNAL(pressed()), this, SLOT(handleStop()));
  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimer()));
  QObject::connect(&tempTimer, SIGNAL(timeout()), this, SLOT(handleTempRead()));
  m_timer.setInterval(1000);

  vbox = new QVBoxLayout();

  QLabel *tempLabel = new QLabel("Temperature");
  tempLabel->setAlignment(Qt::AlignCenter);
  vbox->addWidget(load);
  vbox->addWidget(start);
  vbox->addWidget(stop);
  vbox->addWidget(exit);
  vbox->addWidget(tempLabel);
  vbox->addWidget(tempReading);
  vbox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
  vbox->setSpacing(20);
  buttonPane = new QWidget();
  buttonPane->setLayout(vbox);

  hbox = new QHBoxLayout();
  hbox->addWidget(chartView);
  hbox->addWidget(buttonPane);
  this->setLayout(hbox);
}

void RootWidget::handleExit() {
  std::cout << "Exit" << std::endl;
  if (coater != nullptr) coater->setDefaultState();

  QApplication::quit();
}

void RootWidget::handleStart() {
  std::cout << "Start" << std::endl;
  m_timer.start();
  load->setDisabled(true);
  start->setDisabled(true);
  stop->setDisabled(false);
}

void RootWidget::handleCSV() {
  std::cout << "Load" << std::endl;
  QString filename = QFileDialog::getOpenFileName(this, tr("Open Sequence CSV"),
                                                  "", tr("CSV file (*.csv)"));

  if (filename.isEmpty()) return;

  std::cout << filename.toStdString() << std::endl;

  if (seq != nullptr) delete seq;

  seq = new Sequence(filename.toStdString());
  chart->drawSequence(seq);

  start->setDisabled(false);
}

void RootWidget::handleStop() {
  std::cout << "Stop" << std::endl;
  m_timer.stop();
  chart->reset();
  seq->stop(coater);

  if (coater != nullptr) coater->setDefaultState();

  load->setDisabled(false);
  start->setDisabled(false);
  stop->setDisabled(true);
  timeStep = 0;
}

void RootWidget::handleTimer() {
  if (timeStep < seq->totalTime) {
    chart->step();
    seq->applyState(coater, timeStep, temperature);
  } else {
    handleStop();
    return;
  }
  timeStep++;
}

void RootWidget::handleTempRead() {
  if (tempSensor == nullptr && !tempSensor->isAvailable()) return;
  temperature = tempSensor->readTemperature();
  tempReading->setText(QString::fromStdString(std::to_string(temperature)));
}

void RootWidget::setNanoCoater(NanoCoater *nanoCoater) {
  this->coater = nanoCoater;
}

void RootWidget::setTempSensor(DS18B20 *sensor) {
  if (sensor == nullptr && !sensor->isAvailable()) return;
  tempSensor = sensor;
  tempTimer.setInterval(100);
  tempTimer.start();
}
