#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QtMath>

#include "FFT/headers/qfouriertransformer.h"
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    isSoundActive(false),
    isFrequencyActive(false),
    isLightActive(false),
    timeForPlot(QTime::currentTime()),
    QMainWindow(parent),
    lightSensor(new QLightSensor()),
    lightReader(new QLightReading()),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraph();
    setupTimer();

    lightReader = lightSensor->reading();
    connect((audioInterface.audioInfo).data(), SIGNAL(update()), this, SLOT(realtimeDataSlot2()));

    //this->move(0,0);
    //this->resize(QApplication::desktop()->availableGeometry().width(), QApplication::desktop()->availableGeometry().height());
    //this->showMaximized();
}

MainWindow::~MainWindow()
{
    DeactivateSoundSystem();
    delete ui;
}

void MainWindow::setupGraph()
{
    //sound amplitude graphic
    ui->graphicAmplitude->addGraph(); // blue line

    ui->graphicAmplitude->setNoAntialiasingOnDrag(true);
    ui->graphicAmplitude->setNotAntialiasedElement(QCP::aeAll);

    ui->graphicAmplitude->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    //ui->graphicAmplitude->axisRect()->setupFullAxesBox();
    ui->graphicAmplitude->yAxis->setRange(-100, 100);
    ui->graphicAmplitude->setInteractions(QCP::iRangeDrag);

    //sound waveform graphic
    ui->graphicWaveform->addGraph(); // blue line
    ui->graphicWaveform->addGraph(); // blue line also
    ui->graphicWaveform->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphicWaveform->graph(1)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphicWaveform->graph(0)->setBrush(QBrush(QColor(40, 110, 255, 60)));
    ui->graphicWaveform->graph(1)->setBrush(QBrush(QColor(40, 110, 255, 60)));
    ui->graphicWaveform->axisRect()->setupFullAxesBox();
    ui->graphicWaveform->yAxis->setRange(-100, 100);
    ui->graphicWaveform->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //sound frequency bars
    //ui->graphic2->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphicFrequency->xAxis, ui->graphicFrequency->yAxis);
    //frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphicFrequency->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphicFrequency->axisRect()->setupFullAxesBox();
    ui->graphicFrequency->xAxis->setRange(0, 16);
    ui->graphicFrequency->yAxis->setRange(0, 20);
}

void MainWindow::setupTimer()
{
    //timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::updateSoundDiagram(double x, double y)
{
    ui->graphicAmplitude->graph(0)->addData(x, y);
    // rescale value (vertical) axis to fit the current data:
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphicAmplitude->xAxis->setRange(x, 1, Qt::AlignRight);
    ui->graphicAmplitude->yAxis->rescale(true);
    ui->graphicAmplitude->replot();
/*
    ui->graphicWaveform->graph(0)->addData(x, y);
    ui->graphicWaveform->graph(1)->addData(x, -y);
    ui->graphicWaveform->xAxis->setRange(x, 10, Qt::AlignRight);
    ui->graphicWaveform->yAxis->rescale();
    ui->graphicWaveform->replot();*/
}

void MainWindow::releaseFrequencyBars(QVector<double> x, QVector<double> y)
{
    //sound frequency bars
    ui->graphicFrequency->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphicFrequency->xAxis, ui->graphicFrequency->yAxis);
    frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphicFrequency->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphicFrequency->axisRect()->setupFullAxesBox();
    ui->graphicFrequency->xAxis->rescale();
    ui->graphicFrequency->yAxis->rescale();
    //ui->graphicFrequency->yAxis->setRange(0, 64, Qt::AlignCenter);
    ui->graphicFrequency->replot();
}

void MainWindow::setupWaveform(QVector<double> x, QVector<double> y)
{
  // create graph and assign data to it:
  ui->graphicWaveform->addGraph();
  ui->graphicWaveform->graph(0)->addData(x, y);
  // give the axes some labels:
  ui->graphicWaveform->xAxis->setLabel("x");
  ui->graphicWaveform->yAxis->setLabel("y");
  // set axes ranges, so we see all data:
  ui->graphicWaveform->xAxis->rescale();
  ui->graphicWaveform->yAxis->rescale();
  ui->graphicWaveform->replot();
}



static double lastPointKey = 0;
void MainWindow::realtimeDataSlot()
{
    double key = timeForPlot.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int N = 32;
    static int elementCount = 0;



    const int sampleSize = 32/8;
    static QQueue<double> sample;
    static QQueue<double> keys;

    double amplitude = audioInterface.getValue();

    const double frequency = 44000;
    double interval = 1/frequency;

    double data = amplitude * 500;

    ////////
    /*static QVector<double> X(N), Y(N);
    if(elementCount <= N+1)
    {
        X[elementCount] = elementCount / (double)N;
        Y[elementCount] = data;
        elementCount++;
    }
    if(elementCount == N)
    {
        //FastFourierTransform FFT;
        //FFT.Draw(&Y, N);

        QFourierTransformer QFT(N);
        float Yinf[N], Youtf[N];
        for(int i = 0; i < N; i++)
        {
            Yinf[i] = Y[i];
        }
        QFT.forwardTransform(Yinf, Youtf);
        for(int i = 0; i < N; i++)
        {
            Y[i] = Youtf[i];
        }

        releaseFrequencyBars(X, Y);

        elementCount = 0;
    }*/
    ///////



    //data that will be ploted
    if (key - lastPointKey <= interval)
    {
        return;
    }
    //cout << "key - lastPointKey: " << key - lastPointKey << endl;
    interval = key - lastPointKey;

    if(sample.size() < sampleSize)
    {
        sample.push_back(amplitude);
        keys.push_back(key);
    }
    else
    {
        sample.pop_front();
        sample.push_back(amplitude);
        keys.pop_front();
        keys.push_back(key);
        double w = 2*(M_PI)*(1/interval);
        QVector<double> x(sampleSize), y(sampleSize);
        for(int i = 0; i < sampleSize; i++)
        {
            x[i] = i;
            y[i] = sample[i] * qSin((w * i)/(sampleSize));
        }
        setupWaveform(x, y);
    }


    //double data = 500*log(audioInterface.getValue()+1);

    if(isSoundActive)
    {
        //releaseSoundDiagram(key, data);
    }

    if(isLightActive)
    {

        //ui->lightLabelInfo->setText(QString::number(lightSensor->reading()->lux()));
        //ui->graphic3->
    }

    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    lastPointKey = key;
}


void MainWindow::realtimeDataSlot2()
{
    quint32* sample = audioInterface.getSample();
    const int sampleSize = audioInterface.sampleSize;
    for(int i = 0; i < sampleSize; i++)
    {
        double x = timeForPlot.elapsed()/1000.0;
        double y = sample[i];
        updateSoundDiagram(x, y);
    }
}



void MainWindow::ActivateSoundSystem()
{
    timeForPlot.restart();
    lastPointKey = 0;
    ui->graphicAmplitude->graph()->data()->clear();
    ui->graphicWaveform->graph(0)->data()->clear();
    ui->graphicWaveform->graph(1)->data()->clear();
    ui->graphicWaveform->setInteractions(0);
    ui->graphicAmplitude->setInteractions(0);
    audioInterface.start();
    //timer->start(0); // Interval 0 means to refresh as fast as possible
    ui->startAmplitudeButton->setText("STOP");
    ui->startFrequencyButton->setText("STOP");
    ui->label2->setText("Input on");
    isSoundActive = true;
}

void MainWindow::DeactivateSoundSystem()
{
    //timer->stop();
    audioInterface.stop();
    ui->graphicAmplitude->setInteractions(QCP::iRangeDrag);
    ui->graphicWaveform->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->soundInfoLabel->setText("Press START button");
    ui->startAmplitudeButton->setText("START");
    ui->startFrequencyButton->setText("STOP");
    ui->label2->setText("Input off");
    isSoundActive = false;
}

void MainWindow::soundControlActivate()
{
    if(!isSoundActive)
    {
        ActivateSoundSystem();
    }
    else
    {
        DeactivateSoundSystem();
    }
}

void MainWindow::on_startAmplitudeButton_clicked()
{
    soundControlActivate();
}

void MainWindow::on_startFrequencyButton_clicked()
{
    soundControlActivate();
}
