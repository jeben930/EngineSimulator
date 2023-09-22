#include "pch.h"
#include "EngineSimulatorMain.h"

using namespace EngineSimulator;

EngineSimulatorMain::EngineSimulatorMain() :
    m_windowClosed(false),
    m_windowVisible(true),
    m_engineSimulator()
{
}

void EngineSimulatorMain::Initialize(HWND window, int width, int height)
{
    m_engineSimulator.Initialize();
}

void EngineSimulatorMain::Update()
{
    // Update the engine simulator with the current time
    float currentTime = static_cast<float>(m_timer.GetTotalSeconds());
    m_engineSimulator.Update();
}

void EngineSimulatorMain::SetWindowVisible(bool visible)
{
    m_windowVisible = visible;
}

void EngineSimulatorMain::SetWindowClosed()
{
    m_windowClosed = true;
}

bool EngineSimulatorMain::IsWindowClosed()
{
    return m_windowClosed;
}

bool EngineSimulatorMain::IsWindowVisible()
{
    return m_windowVisible;
}

void EngineSimulatorMain::StartRenderLoop()
{
    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            m_timer.Tick([&]()
                {
                    Update();
                });
        }
        else
        {
            Sleep(100);
        }
    }
}