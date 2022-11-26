#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

// --------------------------
//		DepthStencilBuffer
// --------------------------

void DepthStencilBuffer::Init(const WindowInfo& info)
{
	HRESULT hr;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC dsTex = {};
	dsTex.Width = info.width;
	dsTex.Height = info.height;
	dsTex.MipLevels = 1;
	dsTex.ArraySize = 1;
	dsTex.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsTex.SampleDesc.Count = 1;
	dsTex.SampleDesc.Quality = 0;
	dsTex.Usage = D3D11_USAGE_DEFAULT;
	dsTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsTex.CPUAccessFlags = 0;
	dsTex.MiscFlags = 0;
	hr = DEVICE->CreateTexture2D(&dsTex, nullptr, &_depthStencilTexture);
	CHECK_FAIL(hr, L"Failed Create Depth Stencil Texture");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = dsTex.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = DEVICE->CreateDepthStencilView(_depthStencilTexture, &dsvDesc, &_depthStencilView);
	CHECK_FAIL(hr, L"Failed Create Depth Stencil View");
}

void DepthStencilBuffer::Clear()
{
	if (_depthStencilTexture) _depthStencilTexture = nullptr;
	if (_depthStencilView) _depthStencilView = nullptr;
}
