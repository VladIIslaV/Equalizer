#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QAudioRecorder>
#include <QAmbientLightSensor>
#include <QAmbientLightReading>
#include "qcustomplot.h"
#include "Hardware/audiointerface.h"
#include "View/audioamplitudevizualizer.h"
#include "View/audiofrequencyvizualizer.h"
#include "View/lightamplitudevisualizer.h"

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
    void lightControlActivate();

    bool isSoundActive;
    bool isFrequencyActive;
    bool isLightActive;
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    AudioInterface audioInterface;
    AudioAmplitudeVizualizer* amplitudeVizualizer;
    AudioFrequencyVizualizer* frequencyVizualizer;
    LightAmplitudeVisualizer* lightVisualizer;
    QTime timeForPlot;
    QCPBars* frequencyBars;
    QLightSensor* lightSensor;
    QLightReading* lightReader;
    void ActivateSoundSystem();
    void DeactivateSoundSystem();

    void ActivateLightSystem();
    void DeactivateLightSystem();

public slots:

private slots:
    //void on_startButton3_clicked();
    //void on_startButton3_clicked();
    void on_startAmplitudeButton_clicked();
    void on_startFrequencyButton_clicked();
    void on_startLightButton_clicked();
};

#endif // MAINWINDOW_H
