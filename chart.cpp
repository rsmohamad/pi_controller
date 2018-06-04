#include "chart.h"

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QEasingCurve>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>
#include <QtCharts/QCategoryAxis>

#include <iostream>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    xAxis(new QValueAxis),
    yAxis(new QCategoryAxis)
{
    xAxis->setTickCount(10);
    xAxis->setRange(0, 9);
    setAxisX(xAxis);

    yAxis->append("FILTER_VALVE_CLOSE", 1);
    yAxis->append("FILTER_VALVE_OPEN", 2);
    yAxis->append("NANO_PUMP", 3);
    yAxis->append("ATOM_VALVE_CLOSE", 4);
    yAxis->append("ATOM_VALVE_OPEN", 5);
    yAxis->append("ATOM_PUMP", 6);
    yAxis->append("ATOM_PW", 7);
    yAxis->append("HEATER_TEMP", 8);
    yAxis->append("HEATER_VALVE_CLOSE", 9);
    yAxis->append("HEATER_VALVE_OPEN", 10);
    yAxis->append("HEATER_PUMP", 11);
    yAxis->append("HEATER_PW", 12);

    yAxis->setRange(0, 12);
    yAxis->setGridLineVisible(false);
    yAxis->setShadesPen(Qt::NoPen);
    yAxis->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 30)));
    yAxis->setShadesVisible(true);

    QFont font;
    font.setFamily("Ubuntu");
    yAxis->setLabelsFont(font);

    setAxisY(yAxis);
    setAnimationOptions(QChart::SeriesAnimations);
    setAnimationDuration(100);
}

Chart::~Chart()
{

}

void Chart::step(){
    scroll((plotArea().width()) / (xAxis->tickCount()-1), 0);
}

void Chart::reset(){
    axisX()->setRange(0, 9);
}

void Chart::drawSequence(Sequence *seq){
    series.clear();
    series.resize(seq->state.size());
    removeAllSeries();

    for (unsigned int i = 0; i < seq->state.size(); i++){
        QLineSeries *serie = new QLineSeries(this);
        std::vector<int> points = seq->state[i];

        for (qreal j = 0; j < points.size(); j++){
            qreal y = (11 - i + 0.2);

            // Normalize non binary values
            if (i == ATOM_PUMP || i == HEATER_PUMP)
                y += 0.6 * (double)points[j]/100.0;
            else if (i == NANO_PUMP)
                y += 0.6 * (double)points[j]/2000.0;
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