#include "rootwidget.h"
#include "chart.h"

#include <QtCharts/QChartView>
#include <iostream>
#include <QApplication>
#include <QFileDialog>

RootWidget::RootWidget(QWidget *parent) : QWidget(parent)

{
    chart = new Chart();
    //chart->setTitle("CSV Sequence");
    chart->legend()->hide();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    load = new QPushButton("Load CSV");
    start = new QPushButton("Start");
    stop = new QPushButton("Stop");
    exit = new QPushButton("Exit");

    start->setDisabled(true);
    stop->setDisabled(true);

    QObject::connect(start, SIGNAL(pressed()), this, SLOT(handleStart()));
    QObject::connect(exit, SIGNAL(pressed()), this, SLOT(handleExit()));
    QObject::connect(load, SIGNAL(pressed()), this, SLOT(handleCSV()));
    QObject::connect(stop, SIGNAL(pressed()), this, SLOT(handleStop()));
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimer()));
    m_timer.setInterval(1000);

    vbox = new QVBoxLayout();

    vbox->addWidget(load);
    vbox->addWidget(start);
    vbox->addWidget(stop);
    vbox->addWidget(exit);
    buttonPane = new QWidget();
    buttonPane->setLayout(vbox);

    hbox = new QHBoxLayout();
    hbox->addWidget(chartView);
    hbox->addWidget(buttonPane);
    this->setLayout(hbox);
}

void RootWidget::handleExit(){
    std::cout << "Exit" << std::endl;
    if (coater != nullptr)
        coater->setDefaultState();

    QApplication::quit();
}

void RootWidget::handleStart(){
    std::cout << "Start" << std::endl;
    m_timer.start();
    load->setDisabled(true);
    start->setDisabled(true);
    stop->setDisabled(false);
}

void RootWidget::handleCSV(){
    std::cout << "Load" << std::endl;
    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open Sequence CSV"), "",
            tr("CSV file (*.csv)"));

    if (filename.isEmpty())
        return;

    std::cout << filename.toStdString() << std::endl;

    if (seq != nullptr)
        delete seq;

    seq = new Sequence(filename.toStdString());
    chart->drawSequence(seq);

    start->setDisabled(false);
}

void RootWidget::handleStop(){
    std::cout << "Stop" << std::endl;
    m_timer.stop();
    chart->reset();
    seq->stop(coater);

    if (coater != nullptr)
        coater->setDefaultState();

    load->setDisabled(false);
    start->setDisabled(false);
    stop->setDisabled(true);
    timeStep = 0;
}

void RootWidget::handleTimer(){
    if (timeStep < seq->totalTime){
        chart->step();
        seq->applyState(coater, timeStep);
    }else{
        handleStop();
        return;
    }
    timeStep++;
}

void RootWidget::setNanoCoater(NanoCoater * nanoCoater){
    this->coater = nanoCoater;
}
