#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QAudioRecorder>
#include <QTimer>
#include <complex>
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
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    AudioInterface audioInterface;
    AudioAmplitudeVizualizer* amplitudeVizualizer;
    AudioFrequencyVizualizer* frequencyVizualizer;
    QTime timeForPlot;
    QCPBars* frequencyBars;

    void ActivateSoundSystem();
    void DeactivateSoundSystem();

public slots:

private slots:
    void on_startAmplitudeButton_clicked();
    void on_startFrequencyButton_clicked();
};

#endif // MAINWINDOW_H
