#include "pch.h"
#include "SwapChain.h"
#include "Engine.h"

// ------------------
//		SwapChain
// ------------------

void SwapChain::Init(const WindowInfo& info, shared_ptr<Device> device)
{
	_device = device;
	_info = info;
	CreateSwapChain();
	CreateRTV();
}

void SwapChain::SetRTVDSV()
{
	CONTEXT->OMSetRenderTargets(1, &_renderTargetView, GEngine->GetDSB()->GetDSV());
}

void SwapChain::Present()
{
	_swapChain->Present(0, 0);
}

void SwapChain::CreateSwapChain()
{
	HRESULT hr = S_OK;
	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = _device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		ID3D11Device1* device1;
		ID3D11DeviceContext1* deviceContext;
		hr = _device->GetDevice()->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device1));
		if (SUCCEEDED(hr))
		{
			(void)(_device->GetDeviceContext())->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = _info.width;
		sd.Height = _info.height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		IDXGISwapChain1* swapChain;
		hr = dxgiFactory2->CreateSwapChainForHwnd(_device->GetDevice(), _info.hWnd, &sd, nullptr, nullptr, &swapChain);
		if (SUCCEEDED(hr))
		{
			hr = swapChain->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_swapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = _info.width;
		sd.BufferDesc.Height = _info.height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = _info.hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(_device->GetDevice(), &sd, &_swapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(_info.hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	CHECK_FAIL(hr, L"Failed to Failed to Create SwapChain");
}

void SwapChain::CreateRTV()
{
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return;

	hr = _device->GetDevice()->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
	pBackBuffer->Release();
	CHECK_FAIL(hr, L"Failed to Create RenderTargetView");
}