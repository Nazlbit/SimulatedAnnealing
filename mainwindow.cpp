#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ising_model.h"
#include <random>
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    annealingEngine(5, 0.9999999,
                    [this]{genRandInd(); swapParticles();},
                    [this]{swapParticles();},
                    [this]{ return getDeltaEnergy();})
{
    ui->setupUi(this);
    solution = ui->form->getGrid();

    ui->temperatureText->setText(QString::number(5));
    ui->tRatioText->setText(QString::number(0.9999999, 'g', 7));
    ui->iterationsText->setText(QString::number(1000000));
    ui->energyText->setText(QString::number(solution.getEnergy()));
    ui->widthText->setText("100");
    ui->heightText->setText("100");

    connect(ui->updateButton, &QPushButton::clicked, [this]{
        annealingEngine.tRatio = GetTRatio();
        annealingEngine.temperature = GetTemperature();
        annealingEngine.simulate(GetIterations());

        ui->form->setGrid(solution);
        UpdateTextFields();
    });

    connect(ui->resetButton, &QPushButton::clicked, [this]{
        solution = IsingModel(GetSizeX(), GetSizeY());
        solution.fillRandom();
        ui->form->setGrid(solution);
        annealingEngine.temperature = 5;
        UpdateTextFields();
    });
}

void MainWindow::UpdateTextFields()
{
    ui->temperatureText->setText(QString::number(annealingEngine.temperature));
    ui->energyText->setText(QString::number(solution.getEnergy()));
}

double MainWindow::GetTemperature()
{
    double result = ui->temperatureText->text().toDouble();
    if(result < 0)
    {
        result = 0;
        ui->temperatureText->setText("0");
    }

    return result;
}

double MainWindow::GetTRatio()
{
    double result = ui->tRatioText->text().toDouble();
    if(result < 0)
    {
        result = 0;
        ui->tRatioText->setText("0");
    }

    return result;
}

int64_t MainWindow::GetIterations()
{
    int64_t result = ui->iterationsText->text().toLongLong();
    if(result < 1)
    {
        result = 1;
        ui->iterationsText->setText("1");
    }
    return result;
}

int64_t MainWindow::GetSizeX()
{
    int64_t result = ui->widthText->text().toLongLong();
    if(result < 1)
    {
        result = 1;
        ui->widthText->setText("1");
    }
    return result;
}

int64_t MainWindow::GetSizeY()
{
    int64_t result = ui->heightText->text().toLongLong();
    if(result < 1)
    {
        result = 1;
        ui->heightText->setText("1");
    }
    return result;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::genRandInd()
{
    static std::mt19937_64 g1(static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int64_t> dis_int(0, solution.getSize()-1);

    int64_t u = dis_int(g1);
    int64_t u_raw = u / solution.getWidth();
    int64_t u_column = u % solution.getWidth();

    int64_t v;
    int64_t v_raw;
    int64_t v_column;

    while(true)
    {
        int64_t v = dis_int(g1);
        if(v == u) continue;

        v_raw = v / solution.getWidth();
        v_column = v % solution.getWidth();

        double vx = v_raw - u_raw;
        double vy = v_column - u_column;
        double length = sqrt(vx*vx + vy*vy)*2/sqrt(2);

        vx /= length;
        vy /= length;

        v_raw = u_raw + static_cast<int64_t>(round(vx));
        v_column = u_column + static_cast<int64_t>(round(vy));

        if(u_raw > -1 && u_raw < solution.getHeight() && u_column > -1 && u_column < solution.getWidth())
        {
            break;
        }
    }
    v = solution.getGridIndex(v_raw, v_column);



    particle1Ind = v;
    particle2Ind = u;
    prevLocalEnergy = solution.getLocalEnergy(u) + solution.getLocalEnergy(v);
}

void MainWindow::swapParticles()
{
    std::swap(solution.valueAt(particle1Ind), solution.valueAt(particle2Ind));
}

double MainWindow::getDeltaEnergy()
{
    return solution.getLocalEnergy(particle1Ind) + solution.getLocalEnergy(particle2Ind) - prevLocalEnergy;
}
