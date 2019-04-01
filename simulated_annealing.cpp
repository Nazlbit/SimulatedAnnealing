#include "simulated_annealing.h"
#include <chrono>
#include <random>
#include <QDebug>

SimulatedAnnealing::SimulatedAnnealing(double temperature, double tRatio, std::function<void ()> doActionFunc, std::function<void ()> reverseActionFunc, std::function<double ()> getDeltaEnergyFunc) :
    temperature(temperature),
    tRatio(tRatio),
    doActionFunc(doActionFunc),
    reverseActionFunc(reverseActionFunc),
    getDeltaEnergyFunc(getDeltaEnergyFunc)
{

}

void SimulatedAnnealing::simulate(int64_t iterations)
{
    static std::mt19937_64 g1(static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::uniform_real_distribution<double> dis(0, 1);

    for(int64_t i = 0; i < iterations; i++)
    {
        doActionFunc();
        double delta_energy = getDeltaEnergyFunc();
        double probability = exp(-delta_energy/temperature);
        if(delta_energy >= 0 && probability < dis(g1))
        {
            reverseActionFunc();
        }
        progress++;
        temperature *= tRatio;
    }
}
