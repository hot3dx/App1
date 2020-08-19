//--------------------------------------------------------------------------------------
// File: DirectXPage.xaml.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
//
//--------------------------------------------------------------------------------------
//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System; 
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace concurrency;

DirectXPage::DirectXPage():
	m_windowVisible(true),
	m_coreInput(nullptr)
{
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged += 
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown);

	window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp);
	AppBarButtonStart->Click += ref new RoutedEventHandler(this, &DirectXPage::AppBarButton_Click);
	//IDC_SAVE_BUTTON->Click += ref new RoutedEventHandler(this, &DirectXPage::IDC_SAVE_BUTTON_Click);
	//IDC_SET_COLORS_BUTTON->Click += ref new RoutedEventHandler(this, &DirectXPage::IDC_SET_COLORS_BUTTON_Click);
	
	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this] (IAsyncAction ^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);
		m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerWheelChanged);
		m_coreInput->PointerEntered += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerEntered);
		m_coreInput->PointerExited += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerExited);
		m_coreInput->PointerCaptureLost += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerCaptureLost);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<App1Main>(new App1Main(m_deviceResources));
	m_main->StartRenderLoop();
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_main->StartRenderLoop();
	}
	else
	{
		m_main->StopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

// Called when the app bar button is clicked.
void DirectXPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
	m_deviceResources->m_isSwapPanelVisible = true;
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	if (e->CurrentPoint->Properties->IsLeftButtonPressed) { m_main->GetSceneRender()->Setm_bLButtonDown(true); }
	if (e->CurrentPoint->Properties->IsMiddleButtonPressed) { m_main->GetSceneRender()->Setm_bMButtonDown(true); }
	if (e->CurrentPoint->Properties->IsRightButtonPressed) { m_main->GetSceneRender()->Setm_bRButtonDown(true); }
	// When the pointer is pressed begin tracking the pointer movement.
	m_main->StartTracking();
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	// Update the pointer tracking code.
	if (e->CurrentPoint->Properties->IsLeftButtonPressed) {}
	if (e->CurrentPoint->Properties->IsMiddleButtonPressed) {}
	if (e->CurrentPoint->Properties->IsRightButtonPressed) {}
	if (m_main->IsTracking())
	{
		m_main->TrackingUpdate(e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	}
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	// Stop tracking pointer movement when the pointer is released.
	if (!e->CurrentPoint->Properties->IsLeftButtonPressed) { m_main->GetSceneRender()->Setm_bLButtonDown(false); }
	if (!e->CurrentPoint->Properties->IsMiddleButtonPressed) { m_main->GetSceneRender()->Setm_bMButtonDown(false); }
	if (!e->CurrentPoint->Properties->IsRightButtonPressed) { m_main->GetSceneRender()->Setm_bRButtonDown(false); }

	m_main->StopTracking();
}

void App1::DirectXPage::OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	// negative delta wheel rolled backward - positive delta wheel rolled forward..
	int delta = e->CurrentPoint->Properties->MouseWheelDelta;
}

void App1::DirectXPage::OnPointerEntered(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	
}

void App1::DirectXPage::OnPointerExited(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	
}

void App1::DirectXPage::OnPointerCaptureLost(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	
}

void App1::DirectXPage::OnKeyDown(Windows::UI::Core::CoreWindow^ win/*window*/, Windows::UI::Core::KeyEventArgs^ args)
{
	auto Key = args->VirtualKey;
	switch (Key)
	{
	case VirtualKey::F4:
	{
		//m_windowClosed = true;
#ifdef _DEBUG
		OutputDebugString(TEXT("F4 Pressed\n"));
#endif
	}break;
	case VirtualKey::F5:
	{
		//m_windowClosed = true;
#ifdef _DEBUG
		OutputDebugString(TEXT("F5 Pressed\n"));
#endif
	}
	case VirtualKey::P:       // Pause
	{
#ifdef _DEBUG
		OutputDebugString(TEXT("Pause\n"));
#endif
		//m_pauseKeyActive = true;
	}break;
	case VirtualKey::Home:
	{
#ifdef _DEBUG
		OutputDebugString(TEXT("Home\n"));
#endif
		//m_homeKeyActive = true;
	}break;
	case VirtualKey::Q:
	{
		m_main->GetSceneRender()->Setm_EyeZ(1.0f);
		//m_main->GetSceneRender()->Setm_LookAtZ(1.0f);
		// OutputDebugString(L"Q Pressed\n");
	}break;
	case VirtualKey::W:
	{
		m_main->GetSceneRender()->Setm_EyeY(1.0f);
		m_main->GetSceneRender()->Setm_LookAtY(1.0f);
		// OutputDebugString(L"W Pressed\n");
	}break;
	case VirtualKey::E:
	{
		m_main->GetSceneRender()->Setm_EyeZ(-1.0f);
		//m_main->GetSceneRender()->Setm_LookAtZ(-1.0f);
		// OutputDebugString(L"E Pressed\n");
	}break;
	case VirtualKey::A:
	{
		m_main->GetSceneRender()->Setm_EyeX(1.0f);
		m_main->GetSceneRender()->Setm_LookAtX(1.0f);
		// OutputDebugString(L"A Pressed\n");
	}break;
	case VirtualKey::S:
	{
		m_main->GetSceneRender()->Setm_EyeY(-1.0f);
		m_main->GetSceneRender()->Setm_LookAtY(-1.0f);
		// OutputDebugString(L"S Pressed\n");
	}break;
	case VirtualKey::D:
	{
		m_main->GetSceneRender()->Setm_EyeX(-1.0f);
		m_main->GetSceneRender()->Setm_LookAtX(-1.0f);
		// OutputDebugString(L"D Pressed\n");
	}break;
	case VirtualKey::Z:
	{

		//m_main->GetSceneRender()->m_widthRatio += 0.0001f;

	}break;
	case VirtualKey::X:
	{
		//m_main->GetSceneRender()->m_widthRatio -= 0.0001f;


	}break;
	case VirtualKey::C:
	{
		//m_main->GetSceneRender()->m_heightRatio += 0.0001f;


	}break;
	case VirtualKey::V:
	{
		//m_main->GetSceneRender()->m_heightRatio -= 0.0001f;


	}break;
	case VirtualKey::Left:
	{
	}break;
	case VirtualKey::Right:
	{
	}break;
	case VirtualKey::Up:
	{
	}break;
	case VirtualKey::Down:
	{
	}break;
	}
}

void App1::DirectXPage::OnKeyUp(Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^ args)
{
	if (static_cast<UINT>(args->VirtualKey) < 256)
	{
		//m_pSample->OnKeyUp(static_cast<UINT8>(args->VirtualKey));
	}
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}
