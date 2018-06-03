#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QChartView>
#include <QtCharts/QChart>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtCore/QTimer>

#include "chart.h"
#include "sequence.h"
#include "nanocoater.h"

class RootWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RootWidget(QWidget *parent = nullptr);
    void setNanoCoater(NanoCoater *);

signals:

public slots:
    void handleExit();
    void handleCSV();
    void handleStart();
    void handleStop();
    void handleTimer();

private:
    QWidget *buttonPane;
    QChartView *chartView;

    QHBoxLayout *hbox;
    QVBoxLayout *vbox;

    QPushButton *start;
    QPushButton *exit;
    QPushButton *load;
    QPushButton *stop;

    Chart *chart = nullptr;
    Sequence *seq = nullptr;
    QTimer m_timer;

    int timeStep = 0;
    NanoCoater *coater = nullptr;
};

#endif // MAINWINDOW_H
