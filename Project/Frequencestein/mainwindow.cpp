#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

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
    ui->graphic1->addGraph(); // blue line
    ui->graphic1->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graphic1->axisRect()->setupFullAxesBox();
    ui->graphic1->yAxis->setRange(-100, 100);

    //sound frequency bars
    //ui->graphic2->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphic2->xAxis, ui->graphic2->yAxis);
    //frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphic2->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphic2->axisRect()->setupFullAxesBox();
    ui->graphic2->xAxis->setRange(0, 31);
    ui->graphic2->yAxis->setRange(0, 70);
}


void MainWindow::setupTimer()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::releaseSoundDiagram(double x, double y)
{
    ui->graphic1->graph(0)->addData(x, y);
    // rescale value (vertical) axis to fit the current data:
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graphic1->xAxis->setRange(x, 4, Qt::AlignRight);
    ui->graphic1->graph(0)->rescaleValueAxis();
    ui->graphic1->replot();
}

void MainWindow::releaseFrequencyBars(QVector<double> x, QVector<double> y)
{
    //sound frequency bars
    ui->graphic2->removePlottable(0);
    frequencyBars = new QCPBars(ui->graphic2->xAxis, ui->graphic2->yAxis);
    frequencyBars->setData(x,y);
    frequencyBars->setWidth(1);
    ui->graphic2->addPlottable(frequencyBars);
    QPen pen;
    pen.setWidthF(1);
    pen.setColor(QColor(Qt::black));
    frequencyBars->setPen(Qt::NoPen);
    frequencyBars->setBrush(QColor(205,100,0, 150));
    frequencyBars->setPen(pen);

    ui->graphic2->axisRect()->setupFullAxesBox();
    ui->graphic2->replot();
}

static double lastPointKey = 0;
void MainWindow::realtimeDataSlot()
{
    double key = timeForPlot.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    int N = 32;
    static int elementCount = 0;

    //data that will be ploted
    if (key - lastPointKey <= 0.005) // at most add point every 2 ms
    {
        return;
    }

    double data = audioInterface.getValue()*500;
    //double data = 500*log(audioInterface.getValue()+1);

    if(isSoundActive)
    {
        releaseSoundDiagram(key, data);
    }


    static QVector<double> X(N), Y(N);
    if(elementCount < N)
    {
        X[elementCount] = elementCount - 0.5;
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

        if(isFrequencyActive)
        {
            releaseFrequencyBars(X, Y);
        }
        elementCount = 0;
    }

    if(isLightActive)
    {

        ui->lightLabelInfo->setText(QString::number(lightSensor->reading()->lux()));
        //ui->graphic3->
    }

    ui->soundInfoLabel->setText(QString::number(data)+"dB");
    lastPointKey = key;
}


void MainWindow::on_startButton1_clicked()
{
    if(!isSoundActive)
    {
        timeForPlot.restart();
        lastPointKey = 0;
        ui->graphic1->graph()->data()->clear();
        audioInterface.start();
        timer->start(0); // Interval 0 means to refresh as fast as possible
        ui->startButton1->setText("STOP");
        isSoundActive = true;
    }
    else
    {
        timer->stop();
        audioInterface.stop();
        ui->soundInfoLabel->setText("Press START button");
        ui->startButton1->setText("START");
        isSoundActive = false;
    }
}

void MainWindow::on_startButton2_clicked()
{
    if(!isFrequencyActive)
    {
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        ui->label2->setText("Input on");
        isFrequencyActive = true;
    }
    else
    {
        ui->label2->setText("Input off");
        isFrequencyActive = false;
    }
}

void MainWindow::on_startButton3_clicked()
{
    if(!isLightActive)
    {
        isLightActive = true;
        lightSensor->start();
        lightSensor->reading()->lux();
        //lightReader->lightLevel();
        /*
        timer->start(0); // Interval 0 means to refresh as fast as possible
        audioInterface.start();
        ui->label2->setText("Input on");
        isFrequencyActive = true;*/
    }
    else
    {
        isLightActive = false;
        //ui->label2->setText("Input off");
        //isFrequencyActive = false;
    }
}
