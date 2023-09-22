#include "pch.h"
#include "App.h"
#include "Common\DirectXHelper.h"

using namespace EngineSimulator;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Activation;

// Loads and initializes application assets when the application is loaded.
App::App() :
    m_windowClosed(false),
    m_windowVisible(true),
    m_engine(std::make_unique<EngineSimulatorMain>())
{
}

// Creates and initializes the window when the application is launched.
void App::OnLaunched(LaunchActivatedEventArgs^ args)
{
    // Create a window.
    auto window = CoreWindow::GetForCurrentThread();

    // Register event handlers for app lifecycle.
    window->Activated += ref new TypedEventHandler<CoreWindow^, WindowActivatedEventArgs^>(this, &App::OnActivated);
    window->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);
    window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);
    window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

    // Initialize the sample renderer.
    m_engine->CreateRenderers();
    m_engine->OnWindowSizeChanged(window->Bounds.Width, window->Bounds.Height);

    // Enter the render loop.
    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            m_engine->Update();
            if (m_engine->Render())
            {
                m_deviceResources->Present();
            }
        }
        else
        {
            window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// Called when the window size changes.
void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
    m_engine->OnWindowSizeChanged(args->Size.Width, args->Size.Height);
}

// Called when the app is activated.
void App::OnActivated(CoreWindow^ sender, WindowActivatedEventArgs^ args)
{
    m_engine->OnResuming();
}

// Called when the app is resumed.
void App::OnResuming(Object^ sender, Object^ args)
{
    m_engine->OnResuming();
}

// Called when the app window is closed.
void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
    m_windowClosed = true;
}

// Called when the app window visibility changes.
void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
    m_windowVisible = args->Visible;
    if (m_windowVisible)
    {
        m_engine->OnResuming();
    }
    else
    {
        m_engine->OnSuspending();
    }
}