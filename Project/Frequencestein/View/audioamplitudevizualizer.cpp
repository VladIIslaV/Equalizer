#include "audioamplitudevizualizer.h"

#include <iostream>
using namespace std;

AudioAmplitudeVizualizer::AudioAmplitudeVizualizer(QCustomPlot* inAmplitudeGraph, AudioInterface* inAudioInterface)
{
    amplitudeGraph = inAmplitudeGraph;
    amplitudeGraph->addGraph(amplitudeGraph->xAxis, amplitudeGraph->yAxis2);
    amplitudeGraph->setNoAntialiasingOnDrag(true); // This is for performance improvement.
    amplitudeGraph->setNotAntialiasedElement(QCP::aeAll); // This is for performance improvement.
    QPen pen(QBrush(QColor(40, 110, 255)), 3);
    amplitudeGraph->graph(0)->setPen(pen); // Blue line.
    amplitudeGraph->axisRect()->setAutoMargins(QCP::msNone);
    amplitudeGraph->axisRect()->setMargins(QMargins(0,0,0,0));
    amplitudeGraph->yAxis->setVisible(false);

    amplitudeGraph->yAxis2->setRange(0, 1);
    //amplitudeGraph->yAxis2->setVisible(true);
    amplitudeGraph->yAxis2->grid()->setVisible(true);

    amplitudeGraph->xAxis->setVisible(false);
    amplitudeGraph->xAxis->grid()->setVisible(false);

    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    audioInterface = inAudioInterface;
    connect((audioInterface->audioInfo).data(), SIGNAL(update()), this, SLOT(plotSlot()));
}

void AudioAmplitudeVizualizer::activate()
{
    amplitudeGraph->graph()->data()->clear();
    amplitudeGraph->setInteractions(0);
}

void AudioAmplitudeVizualizer::deactivate()
{
    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void AudioAmplitudeVizualizer::plotSlot()
{
    quint32* amplitudeSample = audioInterface->getSample();
    const int amplitudeSampleSize = audioInterface->sampleSize;
    static double keyPoint = 0;
    double lastKeyPoint = keyPoint;

    for (int i = 0; i < amplitudeSampleSize; i++)
    {
        double x = keyPoint;
        double y = amplitudeSample[i] / (double)INT32_MAX; // divide by (double)INT32_MAX to fit [0..1]
        amplitudeGraph->graph(0)->addData(x, y);
        keyPoint += 0.01;
    }

    static double lastDeletedKeyPoint;
    if(keyPoint - lastDeletedKeyPoint > amplitudeSampleSize)
    {
        lastDeletedKeyPoint = keyPoint - (amplitudeSampleSize * 0.3);
        amplitudeGraph->graph(0)->removeData(0, lastDeletedKeyPoint);
    }

    amplitudeGraph->xAxis->setRange(lastKeyPoint, 3, Qt::AlignRight);
    amplitudeGraph->yAxis2->rescale(true);
    amplitudeGraph->replot();
}
