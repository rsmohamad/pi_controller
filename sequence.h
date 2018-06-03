#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "pindefs.h"
#include "nanocoater.h"

#include <vector>
#include <QPoint>

class Sequence
{
public:
    Sequence(const std::string &filename);
    void stop(NanoCoater *);
    void applyState(NanoCoater *, int timeStep);
    std::vector<std::vector<int>> state;
    int totalTime = 0;

private:
    void generatePlotPoints();
    void readCSV(const std::string &filename);
    void start(NanoCoater *);

    std::vector<std::vector<QPoint>> plotPoints;
    bool isRunning = false;
};

#endif // SEQUENCE_H
