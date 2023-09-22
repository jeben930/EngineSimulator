#pragma once

#include "DeviceResources.h"
#include "Sample3DSceneRenderer.h"
#include "CMBDataset.h"
#include "UniverseSimulator.h"

namespace EngineSimulator
{
    class EngineSimulatorMain
    {
    public:
        EngineSimulatorMain();

        void Initialize(HWND window, int width, int height);
        void Update();
        void SetWindowVisible(bool visible);
        void SetWindowClosed();
        bool IsWindowClosed();
        bool IsWindowVisible();
        void StartRenderLoop();

    private:
        DX::StepTimer m_timer;
        bool m_windowClosed;
        bool m_windowVisible;
        CMBDataset m_cmbDataset;
        UniverseSimulator m_engineSimulator;
    };
}