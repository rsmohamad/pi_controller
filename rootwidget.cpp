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
  manualBtn = new QPushButton("Manual");
  tempReading = new QLabel("0.000");
  tempReading->setAlignment(Qt::AlignCenter);
  QFont f("Arial", 16, QFont::Bold);
  tempReading->setFont(f);

  load->setMinimumSize(100, 70);
  start->setMinimumSize(100, 70);
  stop->setMinimumSize(100, 70);
  exit->setMinimumSize(100, 70);
  manualBtn->setMinimumSize(100, 70);

  start->setDisabled(true);
  stop->setDisabled(true);

  QObject::connect(start, SIGNAL(pressed()), this, SLOT(handleStart()));
  QObject::connect(exit, SIGNAL(pressed()), this, SLOT(handleExit()));
  QObject::connect(load, SIGNAL(pressed()), this, SLOT(handleCSV()));
  QObject::connect(stop, SIGNAL(pressed()), this, SLOT(handleStop()));
  QObject::connect(manualBtn, SIGNAL(pressed()), this, SLOT(handleManual()));
  QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimer()));
  m_timer.setInterval(1000);

  rightVBox = new QVBoxLayout();
  leftVBox = new QVBoxLayout();

  QLabel *tempLabel = new QLabel("Temperature");
  tempLabel->setAlignment(Qt::AlignCenter);
  rightVBox->addWidget(load);
  rightVBox->addWidget(start);
  rightVBox->addWidget(stop);
  rightVBox->addWidget(manualBtn);
  rightVBox->addWidget(exit);
  rightVBox->addWidget(tempLabel);
  rightVBox->addWidget(tempReading);
  rightVBox->setAlignment(Qt::AlignTop | Qt::AlignCenter);
  rightVBox->setSpacing(20);
  rightPane = new QWidget();
  rightPane->setLayout(rightVBox);

  statusButtons.clear();
  for (int i = 0; i < MASTER_SW; i++) {
    QPushButton *btn = new QPushButton();
    QObject::connect(btn, &QPushButton::pressed, this,
                     [=]() { this->handleStatusButton(i); });
    leftVBox->addWidget(btn);
    statusButtons.push_back(btn);
  }

  leftVBox->setAlignment(Qt::AlignCenter);
  leftPane = new QWidget();
  leftPane->setLayout(leftVBox);

  hbox = new QHBoxLayout();
  hbox->addWidget(leftPane);
  hbox->addWidget(chartView);
  hbox->addWidget(rightPane);
  this->setLayout(hbox);

  setStatusButtons(-1);
}

// Override
void RootWidget::paintEvent(QPaintEvent *event) {
  double spacing = statusButtons.back()->height() * MASTER_SW;
  spacing = chart->plotArea().height() - spacing;
  spacing /= MASTER_SW;
  leftVBox->setSpacing(spacing);
  leftVBox->setContentsMargins(0, 0, 0, 20);
}

void RootWidget::setStatusButtons(int timeStep) {
  for (int i = 0; i < MASTER_SW; i++) {
    int status;
    if (timeStep < 0)
      status = 0;
    else
      status = seq->state[i][timeStep];

    std::string text = std::to_string(status);

    if (i != ATOM_PUMP && i != HEATER_PUMP && i != HEATER_TEMP &&
        i != NANO_PUMP)
      text = status == 0 ? "OFF" : "ON";

    statusButtons[i]->setText(QString::fromStdString(text));
    statusButtons[i]->setEnabled(false);
    statusButtons[i]->setStyleSheet(
        "QPushButton:disabled{ background-color: white; color: black}");
  }
}

void RootWidget::handleExit() {
  std::cout << "Exit" << std::endl;
  if (coater != nullptr) coater->setDefaultState();
  gpioTerminate();
  QApplication::quit();
}

void RootWidget::handleStart() {
  std::cout << "Start" << std::endl;
  m_timer.start();
  load->setDisabled(true);
  start->setDisabled(true);
  stop->setDisabled(false);
  manualBtn->setDisabled(true);
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
  setStatusButtons(-1);
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
  manualBtn->setDisabled(false);
  timeStep = 0;
  setStatusButtons(-1);
}

void RootWidget::handleTimer() {
  if (!isManual) {
    if (timeStep < seq->totalTime) {
      chart->step();
      seq->applyState(coater, timeStep, temperature);
      setStatusButtons(timeStep);
    } else {
      handleStop();
      return;
    }
    timeStep++;
  } else {
    manual.tempControl(temperature);
  }
}

void RootWidget::onTemperatureUpdate(double temperature) {
  this->temperature = temperature;
  tempReading->setText(QString::fromStdString(std::to_string(temperature)));
}

void RootWidget::setNanoCoater(NanoCoater *nanoCoater) {
  this->coater = nanoCoater;
  manual.setCoater(nanoCoater);
}

void RootWidget::handleManual() {
  isManual = !isManual;
  setStatusButtons(-1);
  coater->setDefaultState();
  manual.clearState();

  if (!isManual) {
    manualBtn->setText("Manual");
    if (seq != nullptr) start->setEnabled(true);
    load->setEnabled(true);

    coater->setGPIOState(MASTER_SW, 0);
    m_timer.stop();
  } else {
    manualBtn->setText("Quit Manual");
    load->setEnabled(false);
    start->setEnabled(false);
    stop->setEnabled(false);

    coater->setGPIOState(MASTER_SW, 1);
    m_timer.start();

    for (int i = 0; i < MASTER_SW; i++) {
      statusButtons[i]->setEnabled(true);
      statusButtons[i]->setStyleSheet(
          "QPushButton{ background-color: tomato }");
    }
  }
}

void RootWidget::handleStatusButton(int num) {
  if (!isManual) return;
  if (manual.toggleState(num))
    statusButtons[num]->setStyleSheet(
        "QPushButton{ background-color: lightgreen }");
  else
    statusButtons[num]->setStyleSheet(
        "QPushButton{ background-color: tomato }");
  statusButtons[num]->setText(QString::fromStdString(manual.getLabel(num)));
}
