//--------------------------------------------------------------------------------------
// File: pch.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3d12shader.h>
#include <d3d12video.h>
#include "Common\d3dx12.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
// Uses NuGet.org package WinPixEventRuntime 1.0.190604001
#include <pix3.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include "App.xaml.h"

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#if defined(_WIN32) &&  defined(_DEBUG)
#pragma comment(lib, "C://DirectXToolKitXaml12//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib")
#endif
#if defined(NDEBUG) &&  defined(_WIN32)
#pragma comment(lib, "C://DirectXToolKitXaml12//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib")
#endif

#if defined(NDEBUG) &&  defined(_X64)
#pragma comment(lib, "C://DirectXToolKitXaml12//x64//Release//DirectXToolKitXaml12//DirectXToolKitXaml12.lib")
#endif

#if defined(_DEBUG) &&  defined(_X64)
#pragma comment(lib, "C://DirectXToolKitXaml12//x64//Debug//DirectXToolKitXaml12//DirectXToolKitXaml12.lib")
#endif


#include "../DirectXToolKitXaml12/Audio/Audio.h"
#include "../DirectXToolkitXaml12/Graphics/CommonStates.h"
#include "../DirectXToolkitXaml12/Graphics/DirectXHelpers.h"
#include "../DirectXToolkitXaml12/Graphics/DDSTextureLoader.h"
#include "../DirectXToolkitXaml12/Graphics/DescriptorHeap.h"
#include "../DirectXToolkitXaml12/Graphics/Effects.h"
#include "../DirectXToolkitXaml12/Graphics/GeometricPrimitive.h"
#include "../DirectXToolkitXaml12/Graphics/GraphicsMemory.h"
#include "../DirectXToolkitXaml12/Graphics/Model.h"
#include "../DirectXToolkitXaml12/Graphics/PrimitiveBatch.h"
#include "../DirectXToolkitXaml12/Graphics/MyResourceUploadBatch.h"
#include "../DirectXToolkitXaml12/Graphics/RenderTargetState.h"
#include "../DirectXToolkitXaml12/Graphics/SimpleMath.h"
#include "../DirectXToolkitXaml12/Graphics/SpriteBatch.h"
#include "../DirectXToolkitXaml12/Graphics/SpriteFont.h" 
#include "../DirectXToolkitXaml12/Graphics/VertexTypes.h"
