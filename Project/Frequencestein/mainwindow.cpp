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
    //ui->startAmplitudeButton->setText("||");
    ui->startAmplitudeButton->setStyleSheet("background-color: rgb(255, 0, 0);");
    ui->startFrequencyButton->setText("STOP");
    ui->label2->setText("Input on");
    isSoundActive = true;
}

void MainWindow::DeactivateSoundSystem()
{
    audioInterface.stop();
    amplitudeVizualizer->deactivate();
    //ui->soundInfoLabel->setText("Press START button");
    //ui->startAmplitudeButton->setText("|>");
    ui->startAmplitudeButton->setStyleSheet("background-color: rgb(0, 255, 0);");
    ui->startFrequencyButton->setText("START");
    ui->label2->setText("Input off");
    isSoundActive = false;
}

void MainWindow::ActivateLightSystem()
{
    timeForPlot.restart();
    lightVisualizer->activate();
    //ui->startLightButton->setText("STOP");
    //ui->lightInfoLabel->setText("Input on");
    isLightActive = true;
}

void MainWindow::DeactivateLightSystem()
{
    lightVisualizer->deactivate();
    //ui->lightInfoLabel->setText("Press START button");
    //ui->startLightButton->setText("START");
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
