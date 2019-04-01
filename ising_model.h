#ifndef GRID_H
#define GRID_H

#include <cstdint>

class IsingModel
{
public:
    IsingModel() = default;
    IsingModel(int64_t w, int64_t h);
    IsingModel(const IsingModel& model);
    IsingModel(IsingModel&& model);
    ~IsingModel();
    IsingModel& operator=(const IsingModel& model);
    IsingModel& operator=(IsingModel&& model);

    int64_t getHeight() const;
    int64_t getWidth() const;
    int64_t getSize() const;
    int64_t getEnergy() const;
    int64_t getLocalEnergy(int64_t ind1, int64_t ind2) const;
    int64_t getLocalEnergy(int64_t ind) const;
    void fillRandom();
    int64_t getGridIndex(int64_t row, int64_t column) const;
    int8_t& valueAt(int64_t row, const int64_t column);
    int8_t& valueAt(int64_t index);
private:
    int8_t* particles = nullptr;
    int64_t width = 0;
    int64_t height = 0;
};

#endif // GRID_H
