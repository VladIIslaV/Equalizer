#ifndef AUDIOAMPLITUDEVIZUALIZER_H
#define AUDIOAMPLITUDEVIZUALIZER_H
#include <QObject>
#include "qcustomplot.h"
#include "Hardware/audiointerface.h"

class AudioAmplitudeVizualizer : public QObject
{
    Q_OBJECT
private:
    AudioInterface* audioInterface;
    QCustomPlot* amplitudeGraph;

public:
    AudioAmplitudeVizualizer(QCustomPlot* inAmplitudeGraph, AudioInterface* inAudioInterface);
    void activate(), deactivate();
public slots:
    void plotSlot();
};

#endif // AUDIOAMPLITUDEVIZUALIZER_H
