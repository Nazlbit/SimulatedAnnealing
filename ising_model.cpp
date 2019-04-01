#include "ising_model.h"

#include <algorithm>
#include <random>
#include <chrono>

IsingModel::IsingModel(int64_t w, int64_t h)
{
    width = w;
    height = h;
    particles = new int8_t[static_cast<uint64_t>(w*h)];
    fillRandom();
}

IsingModel::IsingModel(const IsingModel &model)
{
    width = model.width;
    height = model.height;
    particles = new int8_t[static_cast<uint64_t>(model.getSize())];
    std::copy(model.particles, model.particles+model.getSize(), particles);
}

IsingModel::IsingModel(IsingModel&& model)
{
    width = model.width;
    height = model.height;
    particles = model.particles;
    model.particles = nullptr;
}

IsingModel::~IsingModel()
{
    if(particles != nullptr)
    {
        delete[] particles;
    }
}

int64_t IsingModel::getHeight() const
{
    return height;
}

int64_t IsingModel::getWidth() const
{
    return width;
}

int64_t IsingModel::getSize() const
{
    return width*height;
}

int64_t IsingModel::getEnergy() const
{
    int64_t energy = 0;
    for(int64_t i = 0; i < height-1; i++)
    {
        for(int64_t j = 0; j < width-1; j++)
        {
            energy -= particles[getGridIndex(i, j)] * particles[getGridIndex(i, j + 1)];
            energy -= particles[getGridIndex(i, j)] * particles[getGridIndex(i + 1, j)];
        }

        energy -= particles[getGridIndex(i, width-1)] * particles[getGridIndex(i + 1, width-1)];
    }

    for(int64_t j = 0; j < width-1; j++)
    {
        energy -= particles[getGridIndex(height-1, j)] * particles[getGridIndex(height-1, j + 1)];
    }
    return energy;
}

int64_t IsingModel::getLocalEnergy(int64_t ind1, int64_t ind2) const
{
    return -particles[ind1]*particles[ind2];
}

int64_t IsingModel::getLocalEnergy(int64_t ind) const
{
    int64_t energy = 0;
    int64_t ind1_row = ind / width;
    int64_t ind1_column = ind % width;
    if(ind1_row-1 >= 0)
    {
        energy += getLocalEnergy(ind, getGridIndex(ind1_row-1, ind1_column));
    }
    if(ind1_row+1 < height)
    {
        energy += getLocalEnergy(ind, getGridIndex(ind1_row+1, ind1_column));
    }
    if(ind1_column-1 >= 0)
    {
        energy += getLocalEnergy(ind, getGridIndex(ind1_row, ind1_column-1));
    }
    if(ind1_column+1 < width)
    {
        energy += getLocalEnergy(ind, getGridIndex(ind1_row, ind1_column+1));
    }
    return energy;
}

void IsingModel::fillRandom()
{
    static std::mt19937 g1(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::uniform_int_distribution<int32_t> dis(0, 1);

    for(int8_t* particle = particles; particle != particles + getSize(); particle++)
    {
        if(dis(g1) == 1)
        {
            *particle = 1;
        }
        else
        {
            *particle = -1;
        }
    }
}

int64_t IsingModel::getGridIndex(int64_t raw, int64_t column) const
{
    return raw * width + column;
}

int8_t& IsingModel::valueAt(int64_t raw, int64_t column)
{
    return particles[getGridIndex(raw, column)];
}

int8_t& IsingModel::valueAt(int64_t index)
{
    return particles[index];
}

IsingModel& IsingModel::operator=(const IsingModel& model)
{
    if(&model != this)
    {
        if(width == model.width && height == model.height)
        {
            std::copy(model.particles, model.particles+model.getSize(), particles);
            return *this;
        }

        if(particles != nullptr)
        {
            delete[] particles;
        }
        width = model.width;
        height = model.height;
        particles = new int8_t[static_cast<uint64_t>(model.getSize())];
        std::copy(model.particles, model.particles+model.getSize(), particles);
    }
    return *this;
}

IsingModel& IsingModel::operator=(IsingModel&& model)
{
    if(&model != this)
    {
        if(particles != nullptr)
        {
            delete[] particles;
        }
        width = model.width;
        height = model.height;
        particles = model.particles;
        model.particles = nullptr;
    }
    return *this;
}
