#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <functional>
#include <cstdint>

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(double temperature, double tRatio, std::function<void()> doActionFunc, std::function<void()> reverseActionFunc, std::function<double()> getDeltaEnergyFunc);
    void simulate(int64_t iterations);
    double temperature;
    double tRatio;
    int64_t progress = 0;
private:
    std::function<void()> doActionFunc;
    std::function<void()> reverseActionFunc;
    std::function<double()> getDeltaEnergyFunc;
};

#endif // SIMULATED_ANNEALING_H
