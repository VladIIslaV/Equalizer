#include "lightamplitudevisualizer.h"

LightAmplitudeVisualizer::LightAmplitudeVisualizer(QCustomPlot *inAmplitudeGraph)
{
    amplitudeGraph = inAmplitudeGraph;
    amplitudeGraph->addGraph();
    amplitudeGraph->setNoAntialiasingOnDrag(true); // This is for performance improvement.
    amplitudeGraph->setNotAntialiasedElement(QCP::aeAll); // This is for performance improvement.
    amplitudeGraph->graph(0)->setPen(QPen(QColor(40, 110, 255))); // Blue line.
    amplitudeGraph->axisRect()->setupFullAxesBox();
    amplitudeGraph->yAxis->setRange(-100, 100);
    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
/*
    lightSensor->start();
    lightReader = lightSensor->reading();
*/
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(plotSlot()));
    timer->start(10);
}

void LightAmplitudeVisualizer::activate()
{
    //amplitudeGraph->graph()->data()->clear();
    amplitudeGraph->setInteractions(0);
}

void LightAmplitudeVisualizer::deactivate()
{
    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void LightAmplitudeVisualizer::plotSlot()
{
    static double keyPoint = 0;
    double lastKeyPoint = keyPoint;

    double x = keyPoint;//timer->remainingTime();
    double y = lightReader->lux();
    amplitudeGraph->graph(0)->addData(x, y);
    keyPoint+= 0.01;
    amplitudeGraph->xAxis->setRange(lastKeyPoint, 4, Qt::AlignRight);
    amplitudeGraph->yAxis->rescale(true);
    amplitudeGraph->replot();
}
