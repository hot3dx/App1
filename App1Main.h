//--------------------------------------------------------------------------------------
// File: App1Main.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SceneRender.h"

// This path works because although App1 and DirectXToolkitXaml12 are
// in a separate folder they are both within the next folder up!
// On your computer this and the /WHOLEARCHIVE folders below must match!!!
// The goal will be to put this library in Windows Kits in the appropriate 10.0.18362.0 folders!
// Or C://Hot3dx
// 
#include <Graphics\AlignedNew.h>
#include <Graphics\RenderTargetState.h>
#include <Graphics\EffectPipelineStateDescription.h>
#include <Graphics\CommonStates.h>
#include <Graphics\GraphicsMemory.h>
#include <Graphics\DescriptorHeap.h>
#include <Graphics\EffectCommon.h>
#include <Graphics\VertexTypes.h>
#include <Graphics\SimpleMath.h>
#include <Graphics\Model.h>
#include <Graphics\PrimitiveBatch.h>
#include <Graphics\GeometricPrimitive.h>
#include <Graphics\SpriteBatch.h>
#include <Graphics\SpriteFont.h>
#include <Graphics\Hot3dxCamera.h>
#include <Audio\Audio.h>
#include <Audio\MediaReader.h>
#include <Graphics\Hot3dxGeometry.h>

/* In Project > App1 Properties > Configuration Properties > Linker > All Options > Additional Options
// This line is for x64//Debug
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//x64//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x64//Release
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//x64//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x86(Win32)//Debug
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// This line is for x86(Win32)//Release
// /WHOLEARCHIVE:C://Users//hot3dx-home//Source//Repos//Xaml//DirectXToolKitXaml12//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib %(AdditionalOptions)
// Each Path is different for each compiled and loinked library
// This switch removes warning LNK4264 caused by 
// In Project > App1 Properties > Configuration Properties > C++ > All Options > Consume Windows Runtime Extensions 
// Yes(/ZW) option
*/


//#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace App1
{
	class App1Main : public DX::IDeviceNotify
	{
	public:
		App1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~App1Main();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();
		void CreateDeviceDependentResources();
		void Clear();
		void OnWindowSizeChanged();
		void ValidateDevice();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();
		void StartTracking() { m_sceneRender->StartTracking(); }
		void TrackingUpdate(float positionX, float positionY) { m_pointerLocationX = positionX; m_pointerLocationY = positionY; }
		void StopTracking() { m_sceneRender->StopTracking(); }
		bool IsTracking() { return m_sceneRender->IsTracking(); }
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		// Accessors
		SceneRender^ GetSceneRender() { return m_sceneRender; }// .get();}
		void SetSceneRender() {
			m_sceneRender = ref new SceneRender(m_deviceResources);
		}
			// std::unique_ptr<SceneRender>(new SceneRender(m_deviceResources)); }

		void PauseRequested() {// if //(m_updateState == App1::UpdateEngineState::Dynamics) 
			m_timer.Stop(); 
			m_pauseRequested = true; };
		void PressComplete() {// if (m_updateState == App1::UpdateEngineState::WaitingForPress) 
			m_pressComplete = true; };


		void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState activationState);

		void KeyDown(Windows::System::VirtualKey key);

		void KeyUp(Windows::System::VirtualKey key);



	private:
		// Process all input from the user before updating game state
		void ProcessInput();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		// std::unique_ptr<SceneRender> m_sceneRender;
		SceneRender^ m_sceneRender;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		///////////////////////////////////////////////////
		bool                                                m_pauseRequested;
		bool                                                m_pressComplete;
		bool                                                m_renderNeeded;
		bool                                                m_haveFocus;
		bool                                                m_visible;
		///////////////////////////////////////////////////
		
		// Track current input pointer position.
		float m_pointerLocationX;
		float m_pointerLocationY;

		// DirectXTK Test objects.
		std::shared_ptr<DirectX::ResourceUploadBatch>                           m_resourceUpload;
		std::unique_ptr<DirectX::GraphicsMemory>                                m_graphicsMemory;
		std::unique_ptr<DirectX::DescriptorHeap>                                m_resourceDescriptors;
		std::unique_ptr<DirectX::CommonStates>                                  m_states;
		std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_artistCameraEffect;
		std::unique_ptr<DirectX::BasicEffect>                                   m_shapeTetraEffect;
		std::unique_ptr<DirectX::Model>                                         m_model;
		std::vector<std::shared_ptr<DirectX::IEffect>>                          m_modelEffects;
		std::unique_ptr<DirectX::EffectTextureFactory>                          m_modelResources;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_artistCamera;
		std::unique_ptr<DirectX::GeometricPrimitive>                            m_shapeTetra;
		std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
		std::unique_ptr<DirectX::SpriteFont>                                    m_smallFont;
		std::unique_ptr<DirectX::SpriteFont>                                    m_font;

		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture1;
		Microsoft::WRL::ComPtr<ID3D12Resource>                                  m_texture2;


		
	};
}