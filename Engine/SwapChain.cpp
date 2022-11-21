#include "pch.h"
#include "SwapChain.h"
#include "Device.h"

// ------------------
//		SwapChain
// ------------------

void SwapChain::Init(const WindowInfo& info, shared_ptr<Device> device)
{
	HRESULT hr = S_OK;

#pragma region SwapChain
	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = device->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
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
		hr = device->GetDevice()->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device1));
		if (SUCCEEDED(hr))
		{
			(void)(device->GetDeviceContext())->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = info.width;
		sd.Height = info.height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		IDXGISwapChain1* swapChain;
		hr = dxgiFactory2->CreateSwapChainForHwnd(device->GetDevice(), info.hWnd, &sd, nullptr, nullptr, &swapChain);
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
		sd.BufferDesc.Width = info.width;
		sd.BufferDesc.Height = info.height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = info.hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(device->GetDevice(), &sd, &_swapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(info.hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return;
#pragma endregion

#pragma region RTV
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return;

	hr = device->GetDevice()->CreateRenderTargetView(pBackBuffer, nullptr, &_renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return;

	device->GetDeviceContext()->OMSetRenderTargets(1, &_renderTargetView, nullptr);
#pragma endregion
}

void SwapChain::Present()
{
	_swapChain->Present(0, 0);
}
