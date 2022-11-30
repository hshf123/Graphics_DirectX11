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
	swapChainDesc.BufferCount = 1;
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
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	hr = ::D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);
	CHECK_FAIL(hr, L"Failed to Create Device And SwapChain");

	ID3D11Texture2D* backBufferPtr;
	// Get the pointer to the back buffer.
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	CHECK_FAIL(hr, L"Failed to Get BackBuffer");

	// Create the render target view with the back buffer pointer.
	hr = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	CHECK_FAIL(hr, L"Failed to Create RenderTargetView");
}

void DeviceAndSwapChain::SetRTVDSV()
{
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, GEngine->GetDSB()->GetDSV());
}

void DeviceAndSwapChain::Present()
{
	_swapChain->Present(1, 0);
}
