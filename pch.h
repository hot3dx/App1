//--------------------------------------------------------------------------------------
// File: pch.h
//
// Copyright (c) Jeff Kubitz - hot3dx. All rights reserved.
// 
// No warranty is expressed or implied use at own risk
//
//--------------------------------------------------------------------------------------

#pragma once
#include <ppltasks.h>

#include <wrl.h>
#include <wrl\client.h>
#include <wrl\implements.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include "Common\d3dx12.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
// Uses NuGet.org package WinPixEventRuntime 1.0.200127001
// and Microsoft.UI.Xaml.2.5.0-prerelease.200812001
#include <pix3.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include "App.xaml.h"
#include <synchapi.h>
#include <strsafe.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include <algorithm>
#include <atomic>
#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapo.h>
#include <hrtfapoapi.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mfmediaengine.h>
#include <mferror.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

#include "Audio/AudioFileReaderXaml12.h"
#include "Audio/XAudio2HelpersXaml12.h"
#include "OmnidirectionalSound.h"
#include "CardioidSound.h"
#include "CustomDecay.h"
#include <Audio/AudioXaml12.h>
#include <Graphics/CommonStatesXaml12.h>
#include <Graphics/DirectXHelpersXaml12.h>
#include <Graphics/DDSTextureLoaderXaml12.h>
#include <Graphics/DescriptorHeapXaml12.h>
#include <Graphics/EffectsXaml12.h>
#include <Graphics/GeometricPrimitiveXaml12.h>
#include <Graphics/GraphicsMemoryXaml12.h>
#include <Graphics/ModelXaml12.h>
#include <Graphics/PrimitiveBatchXaml12.h>
#include <Graphics/MyResourceUploadBatchXaml12.h>
#include <Graphics/RenderTargetStateXaml12.h>
#include <Graphics/SimpleMathXaml12.h>
#include <Graphics/SpriteBatchXaml12.h>
#include <Graphics/SpriteFontXaml12.h> 
#include <Graphics/VertexTypesXaml12.h>

#define SINGLETHREADED FALSE

#define HRTF_2PI    6.283185307f