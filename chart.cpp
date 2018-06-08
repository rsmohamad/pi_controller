#include "chart.h"

#include <QEasingCurve>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>

#include <algorithm>
#include <iostream>
#include <vector>

#include "pindefs.h"

std::string snakeCaseToNormal(std::string input) {
  std::vector<std::string> tokens = split(input, '_');
  std::string output;

  for (std::string token : tokens) {
    for (int i = 1; i < token.length(); i++) token[i] = std::tolower(token[i]);
    output += token + " ";
  }

  return output;
}

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags),
      xAxis(new QValueAxis),
      yAxis(new QCategoryAxis) {
  xAxis->setTickCount(10);
  xAxis->setRange(0, 9);
  setAxisX(xAxis);

  for (int i = 10; i >= 0; i--) {
    std::string label = snakeCaseToNormal(STATE_NAMES[i]);
    yAxis->append(QString::fromStdString(label), 11 - i);
  }

  yAxis->setRange(0, 11);
  yAxis->setGridLineVisible(false);
  yAxis->setShadesPen(Qt::NoPen);
  yAxis->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 30)));
  yAxis->setShadesVisible(true);

  QFont font;
  font.setFamily("Helvetica");
  yAxis->setLabelsFont(font);

  setAxisY(yAxis);
  setAnimationOptions(QChart::SeriesAnimations);
  setAnimationDuration(100);
}

Chart::~Chart() {}

void Chart::step() {
  scroll((plotArea().width()) / (xAxis->tickCount() - 1), 0);
}

void Chart::reset() { axisX()->setRange(0, 9); }

void Chart::drawSequence(Sequence *seq) {
  series.clear();
  series.resize(seq->state.size());
  removeAllSeries();

  for (unsigned int i = 0; i < seq->state.size(); i++) {
    QLineSeries *serie = new QLineSeries(this);
    std::vector<int> points = seq->state[i];

    for (qreal j = 0; j < points.size(); j++) {
      qreal y = (10 - i + 0.2);

      // Normalize non binary values
      if (i == ATOM_PUMP || i == HEATER_PUMP || i == HEATER_TEMP)
        y += 0.6 * (double)points[j] / 100.0;
      else if (i == NANO_PUMP)
        y += 0.6 * (double)points[j] / 2000.0;
      else
        y += 0.6 * (double)points[j];

      serie->append(j, y);
      serie->append(j + 1, y);
    }

    series.push_back(serie);
    addSeries(serie);
    serie->attachAxis(xAxis);
    serie->attachAxis(yAxis);
  }
}
