#include "audioamplitudevizualizer.h"

#include <iostream>
using namespace std;

AudioAmplitudeVizualizer::AudioAmplitudeVizualizer(QCustomPlot* inAmplitudeGraph, AudioInterface* inAudioInterface)
{
    amplitudeGraph = inAmplitudeGraph;
    amplitudeGraph->addGraph();
    amplitudeGraph->setNoAntialiasingOnDrag(true); // This is for performance improvement.
    amplitudeGraph->setNotAntialiasedElement(QCP::aeAll); // This is for performance improvement.
    amplitudeGraph->graph(0)->setPen(QPen(QColor(40, 110, 255))); // Blue line.
    amplitudeGraph->axisRect()->setupFullAxesBox();
    amplitudeGraph->yAxis->setRange(-100, 100);
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
        double y = amplitudeSample[i]; // divide by (double)INT32_MAX to fit [0..1]
        amplitudeGraph->graph(0)->addData(x, y);
        keyPoint += 0.01;
    }

    amplitudeGraph->xAxis->setRange(lastKeyPoint, 4, Qt::AlignRight);
    amplitudeGraph->yAxis->rescale(true);
    amplitudeGraph->replot();
}
