#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

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

    //this->move(0,0);
    //this->resize(QApplication::desktop()->availableGeometry().width(), QApplication::desktop()->availableGeometry().height());
    //this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGraph()
{
    //sound volume graphic
    ui->graphicAmplitude->addGraph(); // blue line
    ui->graphicAmplitude->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphicAmplitude->axisRect()->setupFullAxesBox();
    ui->graphicAmplitude->yAxis->setRange(-100, 100);

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
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::releaseSoundDiagram(double x, double y)
{
    ui->graphicAmplitude->graph(0)->addData(x, y);
    // rescale value (vertical) axis to fit the current data:
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphicAmplitude->xAxis->setRange(x, 4, Qt::AlignRight);
    ui->graphicAmplitude->graph(0)->rescaleValueAxis();
    ui->graphicAmplitude->replot();
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
    ui->graphicFrequency->yAxis->setRange(0, 64, Qt::AlignCenter);
    ui->graphicFrequency->replot();
}

static double lastPointKey = 0;
void MainWindow::realtimeDataSlot()
{
    double key = timeForPlot.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int N = 16;
    static int elementCount = 0;

    double data = audioInterface.getValue()*500;
    //double data = 500*log(audioInterface.getValue()+1);

    //data that will be ploted
    if (key - lastPointKey <= 0.0002) // at most add point every 2 ms
    {
        return;
    }

    if(isSoundActive)
    {
        releaseSoundDiagram(key, data);
    }

    static QVector<double> X(N), Y(N);
    if(elementCount <= N+1)
    {
        X[elementCount] = elementCount;
        Y[elementCount] = data;
        //cout << "Y[" << elementCount << "]: " << data << endl;
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
    }

    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    lastPointKey = key;
}


void MainWindow::soundControlActivate()
{
    if(!isSoundActive)
    {
        timeForPlot.restart();
        lastPointKey = 0;
        ui->graphicAmplitude->graph()->data()->clear();
        audioInterface.start();
        timer->start(0); // Interval 0 means to refresh as fast as possible
        ui->startAmplitudeButton->setText("STOP");
        ui->startFrequencyButton->setText("STOP");
        ui->label2->setText("Input on");
        isSoundActive = true;
    }
    else
    {
        timer->stop();
        audioInterface.stop();
        ui->soundInfoLabel->setText("Press START button");
        ui->startAmplitudeButton->setText("START");
        ui->startFrequencyButton->setText("STOP");
        ui->label2->setText("Input off");
        isSoundActive = false;
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
