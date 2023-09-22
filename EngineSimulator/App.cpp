#include "pch.h"
#include "App.h"
#include "EngineSimulatorMain.h"
#include "Sample3DSceneRenderer.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

using namespace EngineSimulator;

App::App()
{
    m_main = std::make_unique<EngineSimulatorMain>();
}

void App::OnLaunched(LaunchActivatedEventArgs^ args)
{
    UNREFERENCED_PARAMETER(args);

    if (m_window == nullptr)
    {
        m_window = ref new Windows::UI::Core::CoreWindow();
        m_window->VisibilityChanged +=
            ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);
        m_window->Closed +=
            ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);
        m_window->Activate();
    }

    auto deviceResources = std::make_shared<DX::DeviceResources>();
    auto sceneRenderer = std::make_unique<Sample3DSceneRenderer>(deviceResources);
    m_main->StartRenderLoop(m_window, deviceResources, std::move(sceneRenderer));
}

void App::OnActivated(CoreWindow^ window, WindowActivatedEventArgs^ args)
{
    UNREFERENCED_PARAMETER(args);

    auto deviceResources = std::make_shared<DX::DeviceResources>();
    auto sceneRenderer = std::make_unique<Sample3DSceneRenderer>(deviceResources);
    m_main->StartRenderLoop(window, deviceResources, std::move(sceneRenderer));
}

void App::OnVisibilityChanged(CoreWindow^ window, VisibilityChangedEventArgs^ args)
{
    m_main->SetWindowVisible(args->Visible);
}

void App::OnWindowClosed(CoreWindow^ window, CoreWindowEventArgs^ args)
{
    m_main->SetWindowClosed();
}

void App::Run()
{
    CoreApplication::Run(ref new App());
}