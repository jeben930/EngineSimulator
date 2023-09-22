#pragma once

#include "pch.h"

#include <memory>

#include "EngineSimulatorMain.h"
#include "Sample3DSceneRenderer.h"

#include "winrt/Windows.ApplicationModel.Activation.h"
#include "winrt/Windows.UI.Core.h"

#include "Common\DirectXHelper.h"
#include "Common\DeviceResources.h"

#include <Windows.h>
#include <concrt.h>
#include <ppltasks.h>

#include <string>
#include <sstream>
#include <stdexcept>

#include <wrl.h>
#include <Windows.ApplicationModel.Activation.h>

namespace EngineSimulator
{
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		App();

		// IFrameworkView methods.
		virtual void OnLaunched(LaunchActivatedEventArgs^ args);
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		// Application lifecycle event handlers.
		void OnActivated(CoreWindow^ sender, WindowActivatedEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

	private:
		// Private accessor for m_deviceResources, protects against device removed errors.
		std::shared_ptr<DX::DeviceResources> GetDeviceResources();

		std::unique_ptr<EngineSimulatorMain> m_engine;
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<EngineSimulatorMain> m_main;
		bool m_windowClosed;
		bool m_windowVisible;
	};
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
