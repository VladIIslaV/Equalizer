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
    timeForPlot(QTime::currentTime()),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraph();
    setupTimer();

    connect((audioInterface.audioInfo).data(), SIGNAL(update()), this, SLOT(realtimeDataSlot2()));

    /* Open window maximized when application started.
    this->move(0,0);
    this->resize(QApplication::desktop()->availableGeometry().width(), QApplication::desktop()->availableGeometry().height());
    this->showMaximized();
    */
}

MainWindow::~MainWindow()
{
    DeactivateSoundSystem();
    delete ui;
}

void MainWindow::setupGraph()
{
    //sound amplitude graphic
    ui->graphicAmplitude->addGraph();
    ui->graphicAmplitude->setNoAntialiasingOnDrag(true); // This is for performance improvement.
    ui->graphicAmplitude->setNotAntialiasedElement(QCP::aeAll); // This is for performance improvement.
    ui->graphicAmplitude->graph(0)->setPen(QPen(QColor(40, 110, 255))); // Blue line.
    ui->graphicAmplitude->axisRect()->setupFullAxesBox();
    ui->graphicAmplitude->yAxis->setRange(-100, 100);
    ui->graphicAmplitude->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //sound frequency bars
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

// Will we need timer?
void MainWindow::setupTimer()
{
    //timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

<<<<<<< HEAD
// Probably we should divide all releases for different diagrams.
// We have to look performance when amplitude and frequency
// diagrams work together.
=======
void MainWindow::updateSoundDiagram(double x, double y)
{
    ui->graphicAmplitude->graph(0)->addData(x, y);
    // rescale value (vertical) axis to fit the current data:
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphicAmplitude->xAxis->setRange(x, 1, Qt::AlignRight);
    ui->graphicAmplitude->yAxis->rescale(true);
    ui->graphicAmplitude->replot();

    /*ui->graphicWaveform->graph(0)->addData(x, y);
    ui->graphicWaveform->graph(1)->addData(x, -y);
    ui->graphicWaveform->xAxis->setRange(x, 10, Qt::AlignRight);
    ui->graphicWaveform->yAxis->rescale();
    ui->graphicWaveform->replot();*/
}

>>>>>>> Only amplitude reamined.
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

// This is not used but code remained for experiment.
// TODO: Remove this method.
static double lastPointKey = 0;
void MainWindow::realtimeDataSlot()
{
    double key = timeForPlot.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int N = 16;
    static int elementCount = 0;



    const int sampleSize = 32;
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
    if(elementCount == N-1)
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
    }



    //double data = 500*log(audioInterface.getValue()+1);

    if(isSoundActive)
    {
        //releaseSoundDiagram(key, data);
    }



    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    lastPointKey = key;
}

// TODO: Refactor the name of the method.
void MainWindow::realtimeDataSlot2()
{
    quint32* sample = audioInterface.getSample();
    const int sampleSize = audioInterface.sampleSize;
    static double keyPoint = 0;
    double lastKeyPoint = keyPoint;

    for (int i = 0; i < sampleSize; i++)
    {
        double x = keyPoint;
        double y = sample[i];
        ui->graphicAmplitude->graph(0)->addData(x, y);
        keyPoint += 0.01;
    }

    // Right now graps look smooze. We can enable antialiase in setupGraph()
    // to solve it. But will be performance good?
    ui->graphicAmplitude->xAxis->setRange(lastKeyPoint, 4, Qt::AlignRight);
    ui->graphicAmplitude->yAxis->rescale(true);
    ui->graphicAmplitude->replot();
}

// TODO: Delete timer if don't need.
void MainWindow::ActivateSoundSystem()
{
    timeForPlot.restart();
    lastPointKey = 0;
    ui->graphicAmplitude->graph()->data()->clear();
    ui->graphicAmplitude->setInteractions(0);
    audioInterface.start();
    //timer->start(0); // Interval 0 means to refresh as fast as possible
    ui->startAmplitudeButton->setText("STOP");
    ui->startFrequencyButton->setText("STOP");
    ui->label2->setText("Input on");
    isSoundActive = true;
}

// TODO: Delete timer if don't need.
void MainWindow::DeactivateSoundSystem()
{
    //timer->stop();
    audioInterface.stop();
    ui->graphicAmplitude->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
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
