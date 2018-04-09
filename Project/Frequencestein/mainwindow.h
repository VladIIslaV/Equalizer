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
#include "audiointerface.h"
#include "fastfouriertransform.h"

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
    void setupGraph();
    void setupTimer();

    void updateSoundDiagram(double, double);
    void releaseFrequencyBars(QVector<double>, QVector<double>);
    void soundControlActivate();
    void setupWaveform(QVector<double> x, QVector<double> y);

    bool isSoundActive;
    bool isFrequencyActive;
    bool isLightActive;
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    AudioInterface audioInterface;
    QTime timeForPlot;
    QCPBars* frequencyBars;
    QLightSensor* lightSensor;
    QLightReading* lightReader;

    void ActivateSoundSystem();
    void DeactivateSoundSystem();

public slots:
    void realtimeDataSlot();
    void realtimeDataSlot2();
private slots:
    void on_startButton3_clicked();
    void on_startButton3_clicked();
    void on_startAmplitudeButton_clicked();
    void on_startFrequencyButton_clicked();
};

#endif // MAINWINDOW_H
