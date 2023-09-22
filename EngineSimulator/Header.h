#pragma once

#include "CMBDataset.h"

class UniverseSimulator
{
public:
    UniverseSimulator();
    ~UniverseSimulator();

    void Initialize();
    void Update(float deltaTime);
    CMBDataset& GetCMBDataset();

private:
    CMBDataset m_cmbDataset;
};