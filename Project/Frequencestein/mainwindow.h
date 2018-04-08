#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QAudioRecorder>
#include <QTimer>
#include <complex>
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

    void releaseSoundDiagram(double, double);
    void releaseFrequencyBars(QVector<double>, QVector<double>);
    void soundControlActivate();
    void setupWaveform(QVector<double> x, QVector<double> y);

    bool isSoundActive;
    bool isFrequencyActive;
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    AudioInterface audioInterface;
    QTime timeForPlot;
    QCPBars* frequencyBars;

public slots:
    void realtimeDataSlot();
private slots:
    void on_startAmplitudeButton_clicked();
    void on_startFrequencyButton_clicked();
};

#endif // MAINWINDOW_H
