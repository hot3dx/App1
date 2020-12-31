//--------------------------------------------------------------------------------------
// File: DirectXPage.xaml.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------
//
// DirectXPage.xaml.h
// Declaration of the DirectXPage class.
//

#pragma once

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "App1Main.h"

#include <xapo.h>
#include <hrtfapoapi.h>
#include "OmnidirectionalSound.h"

namespace App1
{
	public enum class NotifyType
	{
		StatusMessage,
		ErrorMessage
	};

	/// <summary>
	/// A page that hosts a DirectX SwapChainPanel.
	/// </summary>
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();
		virtual ~DirectXPage();

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void AudioInitialize();
		void AudioStop();

		// Audio Sound
		void SetOnTimerTickDXP(Object^ sender, Object^ e) { OnTimerTickDXP(sender, e); }
		// Audio
		bool Get_initializedDXP() { return _initializedDXP; }
		void Set_initializedDXP(bool b) { _initializedDXP = b; }
		void Start_startDXP() { _startDXP.Start(); }
		void Stop_startDXP() { _startDXP.Stop(); }
		void Start_startOnUpdate(float a, float h, float r) { _startDXP.OnUpdate(a, h, r); }
		//void Start_startDXPSetEnvironment(HrtfEnvironment env) { _startDXP.SetEnvironment(env); }
		Windows::UI::Xaml::DispatcherTimer^ Get_timerDXP() { return _timerDXP; }
		void Set_timerDXP(Windows::UI::Xaml::DispatcherTimer^ timer) { _timerDXP = timer; }
		Windows::Foundation::EventRegistrationToken Get_timerEventTokenDXP() { return _timerEventTokenDXP; }
		void Set_timerEventTokenDXP(Windows::Foundation::EventRegistrationToken token) { _timerEventTokenDXP = token; }
		float Get_radiusDXP() { return _radiusDXP; }  // Radius of the orbit
		void  Set_radiusDXP(float radius) { _radiusDXP = radius; }
		float Get_heightDXP() { return _heightDXP; } // Height at which the sound is orbiting (0 for centered around listener's head, +ve for above and -ve for below)
		void Set_heightDXP(float height) { _heightDXP = height; }
		float Get_angularVelocityDXP() { return _angularVelocityDXP; } // Speed of orbit, default is stationary
		void Set_angularVelocityDXP(float angular) { _angularVelocityDXP = angular; }

	internal:
		void NotifyUser(Platform::String^ strMessage, NotifyType type);

	private:
		// XAML low-level rendering event handler.
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers.
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

		// DisplayInformation event handlers.
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Other event handlers.
		void AppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		// Track our independent input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerEntered(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerExited(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerCaptureLost(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		void OnKeyDown(Windows::UI::Core::CoreWindow^ /*window*/, Windows::UI::Core::KeyEventArgs^ args);
		void OnKeyUp(Windows::UI::Core::CoreWindow^ /*window*/, Windows::UI::Core::KeyEventArgs^ args);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<App1Main> m_main; 
		bool m_windowVisible;
		int cnt = 0;

		// Audio
		void OnTimerTickDXP(Object^ sender, Object^ e);
		
		bool                            _initializedDXP = false;
		OmnidirectionalSound            _startDXP;
		Windows::UI::Xaml::DispatcherTimer^ _timerDXP;
		Windows::Foundation::EventRegistrationToken          _timerEventTokenDXP;
		float                           _radiusDXP = 2.0f;            // Radius of the orbit
		float                           _heightDXP = 0;               // Height at which the sound is orbiting (0 for centered around listener's head, +ve for above and -ve for below)
		float                           _angularVelocityDXP = 0;      // Speed of orbit, default is stationary

	};
}

