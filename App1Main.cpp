//--------------------------------------------------------------------------------------
// File: App1Main.cpp
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "..\Common\DeviceResources.h"
#include "App1Main.h"
#include "Content\SceneRender.h"
#include "Common\DirectXHelper.h"
#include <ppltasks.h>
#include <synchapi.h>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <system_error>
#include <thread>
#include <utility>
#include "DirectXPage.xaml.h"
#include <strsafe.h>

using namespace App1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml; 
using namespace Concurrency;

//#pragma comment(lib, "C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//x64//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib")

// Loads and initializes application assets when the application is loaded.
App1Main::App1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f), m_pointerLocationY(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	SetSceneRender();
	//m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
	//m_controller = ref new Hot3dx::Hot3dxController(Window::Current->CoreWindow, m_deviceResources->GetSwapChainPanel()->Dispatcher);

	//m_controller->AutoFire(false);
}

App1Main::~App1Main() 
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
	
	
}

void App1::App1Main::CreateDeviceDependentResources()
{
	//m_sceneRenderer = std::unique_ptr<SceneRenderer>(new SceneRenderer(m_deviceResources));
	m_sceneRender->CreateDeviceDependentResources();

}

// Updates application state when the window size changes (e.g. device orientation change)
void App1Main::CreateWindowSizeDependentResources() 
{
	// Register event handlers for page lifecycle.

		// TODO: Replace this with the size-dependent initialization of your app's content.
	if (m_sceneRender->GetLoadingComplete() == false)return;
	m_sceneRender->CreateWindowSizeDependentResources();
	
}

void App1Main::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction^ action)
		{
			while (action->Status == Windows::Foundation::AsyncStatus::Started)
			{
				critical_section::scoped_lock lock(m_criticalSection);
				////////////////////////////////////////////////////////////////////

				auto commandQueue = m_deviceResources->GetCommandQueue();
				PIXBeginEvent(commandQueue, 0, L"Update");
				{
					Update();
				}
				PIXEndEvent(commandQueue);

				PIXBeginEvent(commandQueue, 0, L"Render");
				{
					//if (Render() == true)
					//{

						//m_deviceResources->Present();
						//m_sceneRenderer->SetLoadingComplete(false);
					if (m_deviceResources->m_isSwapPanelVisible == true)
					{
						m_sceneRender->Render();
					}
					//OutputDebugString(L"\nAppXamlDX12:Rendered\n");

					//	
				//}

				}
				PIXEndEvent(commandQueue);


			}
		});
	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void App1Main::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void App1Main::Update() 
{
	ProcessInput();
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRender->Update(m_timer);
		//m_fpsTextRenderer->Update(m_timer);
	});

	Render();
}


// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool App1Main::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	if (!m_sceneRender->GetLoadingComplete())
	{
		return false;
	}


	//auto 
	ID3D12GraphicsCommandList* context = m_deviceResources->GetCommandList();// m_cd3d12GridRenderer->GetComList();// 

	if (context == 0x0000000000000000)
	{
		return m_sceneRender->Render();
	}

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	if (m_deviceResources->m_isSwapPanelVisible == true)
	{
		return m_sceneRender->Render();
	}

	return true;
}

void App1::App1Main::Clear()
{
	m_sceneRender->Clear();
}

void App1::App1Main::OnWindowSizeChanged()
{
	Size size = m_deviceResources->GetOutputSize();
	return;
	if (!m_deviceResources->WindowSizeChanged(
		(int)size.Width,
		(int)size.Height,
		m_deviceResources->GetRotation()))
		return;
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRender->CreateWindowSizeDependentResources();

}

void App1::App1Main::ValidateDevice()
{
	m_deviceResources->ValidateDevice();
}

void App1::App1Main::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.

    // Process lifetime management may terminate suspended apps at any time, so it is
    // good practice to save any state that will allow the app to restart where it left off.

    m_sceneRender->SaveState();
	
	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.

}

void App1::App1Main::OnResuming()
{
	// TODO: Replace this with your app's resuming logic.
	m_timer.ResetElapsedTime();
	/*m_gamePadButtons.Reset();
	m_keyboardButtons.Reset();
	m_audEngine->Resume();*/
}

void App1::App1Main::OnDeviceRemoved()
{
	// TODO: Save any necessary application or renderer state and release the renderer
	// and its resources which are no longer valid.
	   m_sceneRender->SaveState();
	   m_sceneRender = nullptr;
}

// Notifies renderers that device resources need to be released.
void App1Main::OnDeviceLost()
{
	m_sceneRender->OnDeviceLost();
	//m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void App1Main::OnDeviceRestored()
{
	m_sceneRender->OnDeviceRestored();
}

void App1::App1Main::WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState)
{  /*
	if (activationState == CoreWindowActivationState::Deactivated)
	{
		m_haveFocus = false;

		switch (m_updateState)
		{
		case UpdateEngineState::Dynamics:
			// From Dynamic mode, when coming out of Deactivated rather than going directly back into Graphic play
			// go to the paused state waiting for user input to continue
			m_updateStateNext = UpdateEngineState::WaitingForPress;
			m_pressResult = PressResultState::ContinueLevel;
			SetGraphicInfoOverlay(GraphicInfoOverlayState::Pause);
			m_uiControl->ShowGraphicInfoOverlay();
			m_graphic->PauseGraphic();
			m_updateState = UpdateEngineState::Deactivated;
			m_uiControl->SetAction(GraphicInfoOverlayCommand::None);
			m_renderNeeded = true;
			break;

		case UpdateEngineState::WaitingForResources:
		case UpdateEngineState::WaitingForPress:
			m_updateStateNext = m_updateState;
			m_updateState = UpdateEngineState::Deactivated;
			m_uiControl->SetAction(GraphicInfoOverlayCommand::None);
			m_uiControl->ShowGraphicInfoOverlay();
			m_renderNeeded = true;
			break;
		}
		// Don't have focus so shutdown input processing
		m_controller->Active(false);
	}
	else if (activationState == CoreWindowActivationState::CodeActivated
		|| activationState == CoreWindowActivationState::PointerActivated)
	{
		m_haveFocus = true;

		if (m_updateState == UpdateEngineState::Deactivated)
		{
			m_updateState = m_updateStateNext;

			if (m_updateState == UpdateEngineState::WaitingForPress)
			{
				m_uiControl->SetAction(GraphicInfoOverlayCommand::TapToContinue);
				m_controller->WaitForPress(m_renderer->GraphicInfoOverlayUpperLeft(), m_renderer->GraphicInfoOverlayLowerRight());
			}
			else if (m_updateStateNext == UpdateEngineState::WaitingForResources)
			{
				m_uiControl->SetAction(GraphicInfoOverlayCommand::PleaseWait);
			}
		}
	}*/
}

void App1::App1Main::KeyDown(Windows::System::VirtualKey key)
{
}

void App1::App1Main::KeyUp(Windows::System::VirtualKey key)
{
}

void App1::App1Main::ProcessInput()
{
	// TODO: Add per frame input handling here.
		m_sceneRender->TrackingUpdate(m_pointerLocationX, m_pointerLocationY);
	
}
