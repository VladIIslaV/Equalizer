#include "lightamplitudevisualizer.h"

LightAmplitudeVisualizer::LightAmplitudeVisualizer(QCustomPlot *inAmplitudeGraph)
{
    QPen my_pen(QColor(255, 0, 0));
    my_pen.setWidth(5);

    amplitudeGraph = inAmplitudeGraph;
    amplitudeGraph->addGraph();
    amplitudeGraph->setNoAntialiasingOnDrag(true); // This is for performance improvement.
    amplitudeGraph->setNotAntialiasedElement(QCP::aeAll); // This is for performance improvement.
    amplitudeGraph->graph(0)->setPen(my_pen); // Red line.
    amplitudeGraph->axisRect()->setupFullAxesBox();
    amplitudeGraph->yAxis->setRange(0, 1000);
    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    lightSensor = new QLightSensor(this);

    lightSensor->start();
    lightReader = lightSensor->reading();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(plotSlot()));
    timer->start(10);
}

void LightAmplitudeVisualizer::activate()
{
    amplitudeGraph->graph()->data()->clear();
    //timer->start(10);
    amplitudeGraph->setInteractions(0);
}

void LightAmplitudeVisualizer::deactivate()
{
    //timer->stop();
    amplitudeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void LightAmplitudeVisualizer::plotSlot()
{
    static double keyPoint = 0;
    double lastKeyPoint = keyPoint;

    double x = keyPoint;//timer->remainingTime();
    double y = lightReader->lux();
    y = y < 900 ? y : 900;
    amplitudeGraph->graph(0)->addData(x, y);
    keyPoint+= 0.01;
    amplitudeGraph->xAxis->setRange(lastKeyPoint, 4, Qt::AlignRight);
    //amplitudeGraph->yAxis->rescale(false);
    amplitudeGraph->replot();
}
