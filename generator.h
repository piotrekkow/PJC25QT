#pragma once
#include <qtypes.h>

class Generator
{
public:
    ~Generator() = default;
};

class Roadway;

class VehicleGenerator : public Generator
{
    const Roadway* roadway_;

public:
    VehicleGenerator(qreal rate);

    void update(qreal deltaTime);
};
