#ifndef AUDIOFREQUENCYVIZUALIZER_H
#define AUDIOFREQUENCYVIZUALIZER_H

#include <QObject>
#include "qcustomplot.h"
#include "audiointerface.h"
#include "fastfouriertransform.h"

class AudioFrequencyVizualizer : public QObject
{
    Q_OBJECT
private:
    AudioInterface* audioInterface;
    QCustomPlot* frequencyGraph;
    QCPBars* frequencyBars;

public:
    explicit AudioFrequencyVizualizer(QCustomPlot* inFrequencyGraph, AudioInterface* inAudioInterface, QObject *parent = nullptr);
    void activate(), deactivate();

signals:

public slots:
    void plotSlot();
};

#endif // AUDIOFREQUENCYVIZUALIZER_H
