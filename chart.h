#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

#include "sequence.h"

#include <vector>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QCategoryAxis;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();
    void reset();
    void step();
    void drawSequence(Sequence *seq);

private:
    QTimer m_timer;
    QStringList m_titles;
    QValueAxis *xAxis;
    QCategoryAxis *yAxis;

    std::vector<QLineSeries *> series;
};

#endif // CHART_H
