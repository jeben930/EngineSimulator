#include "pch.h"
#include "EngineSimulatorMain.h"

using namespace EngineSimulator;

EngineSimulatorMain::EngineSimulatorMain() :
    m_deviceResources(std::make_shared<DX::DeviceResources>())
{
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
}

void EngineSimulatorMain::CreateRenderers()
{
    m_sceneRenderer = std::make_unique<Sample3DSceneRenderer>(m_deviceResources);
}

void EngineSimulatorMain::OnWindowSizeChanged(int width, int height)
{
    m_deviceResources->SetLogicalSize(DirectX::XMFLOAT2(static_cast<float>(width), static_cast<float>(height)));
    m_sceneRenderer->CreateWindowSizeDependentResources();
}

void EngineSimulatorMain::Update()
{
    m_timer.Tick([&]()
        {
            // Update scene objects.
            m_sceneRenderer->Update(m_timer);
        });
}

void EngineSimulatorMain::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Render the scene.
    m_sceneRenderer->Render();

    // Present the frame.
    m_deviceResources->Present();
}

void EngineSimulatorMain::OnDeviceLost()
{
    m_sceneRenderer->ReleaseDeviceDependentResources();
}

void EngineSimulatorMain::OnDeviceRestored()
{
    m_sceneRenderer->CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void EngineSimulatorMain::CreateWindowSizeDependentResources()
{
    m_sceneRenderer->CreateWindowSizeDependentResources();
}

void EngineSimulatorMain::OnSuspending()
{
    m_deviceResources->Trim();
}

void EngineSimulatorMain::OnResuming()
{
    m_deviceResources->HandleDeviceLost();
}