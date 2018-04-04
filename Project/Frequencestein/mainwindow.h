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

    void releaseSoundDiagram(double, double);
    void releaseFrequencyBars(QVector<double>, QVector<double>);

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
public slots:
    void realtimeDataSlot();
private slots:
    void on_startButton1_clicked();
    void on_startButton2_clicked();
    void on_startButton3_clicked();
};

#endif // MAINWINDOW_H
