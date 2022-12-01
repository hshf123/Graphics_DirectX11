#include "pch.h"
#include "DeviceAndSwapChain.h"
#include "Engine.h"

// ----------------------------
//		DeviceAndSwapChain
// ----------------------------

void DeviceAndSwapChain::Init(const WindowInfo& info)
{
	_info = info;

	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = static_cast<uint32>(_info.width);
	swapChainDesc.BufferDesc.Height = static_cast<uint32>(_info.height);
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _info.hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = _info.windowed;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	uint32 createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	uint32 numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	uint32 numFeatureLevels = ARRAYSIZE(featureLevels);

	for (uint32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = ::D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, &_featureLevel, &_deviceContext);
		CHECK_FAIL(hr, L"Failed to Create Device And SwapChain");

		if (SUCCEEDED(hr))
			break;
	}

	D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwOpts;
	hr = _device->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwOpts, sizeof(hwOpts));	
	if (hwOpts.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
	{
		return;
	}


}

void DeviceAndSwapChain::Present()
{
	_swapChain->Present(1, 0);
}
