#ifndef LIGHTAMPLITUDEVISUALIZER_H
#define LIGHTAMPLITUDEVISUALIZER_H
#include <QObject>
#include "qcustomplot.h"
#include <QLightSensor>
#include <QLightReading>
#include <QTimer>

class LightAmplitudeVisualizer : public QObject
{
    Q_OBJECT
private:
    QLightSensor* lightSensor;
    QLightReading* lightReader;
    QCustomPlot* amplitudeGraph;
    QTimer* timer;

public:
    LightAmplitudeVisualizer(QCustomPlot* inAmplitudeGraph);
    void activate(), deactivate();
public slots:
    void plotSlot();
};

#endif // LIGHTAMPLITUDEVISUALIZER_H
