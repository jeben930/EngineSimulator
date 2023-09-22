#include "pch.h"
#include "UniverseSimulator.h"

UniverseSimulator::UniverseSimulator() : m_cmbDataset()
{
}

UniverseSimulator::~UniverseSimulator()
{
}

void UniverseSimulator::Initialize()
{
    // Initialize the CMBDataset model
    m_cmbDataset.initialize(1, 18, 4000000000);
}

void UniverseSimulator::Update()
{
    // Update the CMBDataset model with the current time
    m_cmbDataset.update_grid();
}

CMBDataset& UniverseSimulator::GetCMBDataset()
{
    return m_cmbDataset;
}