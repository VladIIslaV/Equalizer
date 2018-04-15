#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QtMath>
#include <limits>

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
    setupGraphs();

    lightReader = lightSensor->reading();
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

void MainWindow::setupGraphs()
{
    amplitudeVizualizer = new AudioAmplitudeVizualizer(ui->graphicAmplitude, &audioInterface);
    frequencyVizualizer = new AudioFrequencyVizualizer(ui->graphicFrequency, &audioInterface);
}

void MainWindow::ActivateSoundSystem()
{
    timeForPlot.restart();
    amplitudeVizualizer->activate();
    audioInterface.start();
    ui->startAmplitudeButton->setText("STOP");
    ui->startFrequencyButton->setText("STOP");
    ui->label2->setText("Input on");
    isSoundActive = true;
}

void MainWindow::DeactivateSoundSystem()
{
    audioInterface.stop();
    amplitudeVizualizer->deactivate();
    ui->soundInfoLabel->setText("Press START button");
    ui->startAmplitudeButton->setText("START");
    ui->startFrequencyButton->setText("START");
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
