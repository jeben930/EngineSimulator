#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Sample3DSceneRenderer.h"

namespace EngineSimulator
{
    class EngineSimulatorMain
    {
    public:
        EngineSimulatorMain();
        void CreateRenderers();
        void OnWindowSizeChanged(int width, int height);
        void Update();
        bool Render();
        void OnSuspending();
        void OnResuming();
        void OnDeviceRemoved();

    private:
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
        DX::StepTimer m_timer;
    };
}