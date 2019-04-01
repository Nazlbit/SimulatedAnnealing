#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulated_annealing.h"
#include "ising_model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SimulatedAnnealing annealingEngine;
    IsingModel solution;
    int64_t particle1Ind;
    int64_t particle2Ind;
    double prevLocalEnergy;

    void genRandInd();
    void swapParticles();
    double getDeltaEnergy();
    void UpdateTextFields();
    double GetTemperature();
    double GetTRatio();
    int64_t GetIterations();
    int64_t GetSizeX();
    int64_t GetSizeY();
};

#endif // MAINWINDOW_H
