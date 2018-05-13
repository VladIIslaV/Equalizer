#include "audiofrequencyvizualizer.h"

AudioFrequencyVizualizer::AudioFrequencyVizualizer(QCustomPlot* inFrequencyGraph, AudioInterface* inAudioInterface, QObject *parent) : QObject(parent)
{
    frequencyGraph = inFrequencyGraph;
    frequencyBars = new QCPBars(frequencyGraph->xAxis, frequencyGraph->yAxis);
    frequencyBars->setWidth(0.33);
    frequencyGraph->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    //frequencyGraph->axisRect()->setAutoMargins(QCP::msNone);
    //frequencyGraph->axisRect()->setMargins(QMargins(0,0,0,0));
    frequencyGraph->yAxis->setVisible(false);
    frequencyGraph->yAxis->setRange(0, 20);

    frequencyGraph->xAxis->setLabel("kHz");
    frequencyGraph->xAxis->setRange(0, 16);
    frequencyGraph->xAxis->grid()->setVisible(false);

    audioInterface = inAudioInterface;
    connect((audioInterface->audioInfo).data(), SIGNAL(update()), this, SLOT(plotSlot()));
}

void AudioFrequencyVizualizer::activate()
{
    // While does nothing.
}

void AudioFrequencyVizualizer::deactivate()
{
    // While do nothing.
}

void AudioFrequencyVizualizer::plotSlot()
{
    quint32* sample = audioInterface->getSample();
    const int sampleSize = audioInterface->sampleSize;

    const int frequencySampleMultiplier = 2;
    const int frequencySampleSize = frequencySampleMultiplier * sampleSize;
    static int frequenceThresholdIndex = 0;
    static int frequencySampleIndex = 0;
    static QVector<double> frequencySample(frequencySampleSize);

    for (int i = 0; i < sampleSize; i++)
    {
        if(frequencySampleIndex < frequencySampleSize)
        {
            frequencySample[frequencySampleIndex] = (sample[i] / (double)INT32_MAX) * 2; // Multiply by two to maximize.
            frequencySampleIndex++;
        }
    }

    if (frequenceThresholdIndex < frequencySampleMultiplier)
    {
        frequenceThresholdIndex++;
    }
    else if (frequenceThresholdIndex == frequencySampleMultiplier)
    {
        FastFourierTransform FFT(frequencySampleSize);
        FFT.Draw(&frequencySample);

        QVector<double> X(frequencySampleSize);
        for(int i = 0; i < frequencySampleSize/2; i++)
        {
            X[i] = i * 0.3333;                                  // Convert to frequency in kHz.
            frequencySample[i] = pow(frequencySample[i], 2);    // Power of 2 for maximize.
        }
        for(int i = frequencySampleSize/2; i < frequencySampleSize; i++)
        {
            X[i] = (frequencySampleSize - i) * 0.3333;          // Convert to frequency in kHz.
            frequencySample[i] = pow(frequencySample[i], 2);    // Power of 2 for maximize.
        }
        frequencySample[0] = 0;   // The 0 element is always very big.

        frequencyBars->setData(X, frequencySample);
        frequencyGraph->xAxis->rescale();
        frequencyGraph->yAxis->rescale();
        frequencyGraph->replot();

        frequenceThresholdIndex = 0;
        frequencySampleIndex = 0;
    }
}
