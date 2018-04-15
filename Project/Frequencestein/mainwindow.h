#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QAudioRecorder>
#include <QTimer>
#include <complex>
#include <QAmbientLightSensor>
#include <QAmbientLightReading>
#include <QLightSensor>
#include <QLightReading>
#include "qcustomplot.h"
#include "Hardware/audiointerface.h"
#include "View/audioamplitudevizualizer.h"
#include "View/audiofrequencyvizualizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void setupGraphs();
    void soundControlActivate();

    bool isSoundActive;
    bool isFrequencyActive;
    bool isLightActive;
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    AudioInterface audioInterface;
    AudioAmplitudeVizualizer* amplitudeVizualizer;
    AudioFrequencyVizualizer* frequencyVizualizer;
    QTime timeForPlot;
    QCPBars* frequencyBars;
    QLightSensor* lightSensor;
    QLightReading* lightReader;

    void ActivateSoundSystem();
    void DeactivateSoundSystem();

public slots:

private slots:
    //void on_startButton3_clicked();
    //void on_startButton3_clicked();
    void on_startAmplitudeButton_clicked();
    void on_startFrequencyButton_clicked();
};

#endif // MAINWINDOW_H
