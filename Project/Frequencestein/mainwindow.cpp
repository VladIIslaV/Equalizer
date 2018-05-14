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
    //lightSensor(new QLightSensor(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraphs();

    //ui->scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    //lightSensor->start();
    //lightReader = lightSensor->reading();

    //connect((audioInterface.audioInfo).data(), SIGNAL(update()), this, SLOT(realtimeDataSlot2()));

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
    lightVisualizer = new LightAmplitudeVisualizer(ui->graphicLight);
}

void MainWindow::ActivateSoundSystem()
{
    timeForPlot.restart();
    amplitudeVizualizer->activate();
    audioInterface.start();
    ui->startAmplitudeButton->setStyleSheet("border: none; background-color: rgb(255, 0, 0);");
    ui->startFrequencyButton->setStyleSheet("border: none; background-color: rgb(255, 0, 0);");
    ui->startAmplitudeButton->setText("STOP");
    ui->startFrequencyButton->setText("STOP");
    isSoundActive = true;
}

void MainWindow::DeactivateSoundSystem()
{
    audioInterface.stop();
    amplitudeVizualizer->deactivate();
    ui->startAmplitudeButton->setStyleSheet("border: none; background-color: rgb(0, 255, 0);");
    ui->startFrequencyButton->setStyleSheet("border: none; background-color: rgb(0, 255, 0);");
    ui->startAmplitudeButton->setText("START");
    ui->startFrequencyButton->setText("START");
    isSoundActive = false;
}

void MainWindow::ActivateLightSystem()
{
    timeForPlot.restart();
    lightVisualizer->activate();
    isLightActive = true;
}

void MainWindow::DeactivateLightSystem()
{
    lightVisualizer->deactivate();
    isLightActive = false;
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

void MainWindow::lightControlActivate()
{
    if(!isLightActive)
    {
        ActivateLightSystem();
    }
    else
    {
        DeactivateLightSystem();
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

void MainWindow::on_startLightButton_clicked()
{
    lightControlActivate();
}
