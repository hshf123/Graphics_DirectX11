#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

// ------------------------
//		ConstantBuffer
// ------------------------

void ConstantBuffer::Init(uint32 size)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd = {};
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = DEVICE->CreateBuffer(&bd, nullptr, &_constantBuffer);
	CHECK_FAIL(hr, L"Failed Create Constant Buffer");
}

void ConstantBuffer::PushData(uint32 startSlot, void* buffer)
{
	DEVICECTX->UpdateSubresource(_constantBuffer, 0, nullptr, buffer, 0, 0);
	DEVICECTX->VSSetConstantBuffers(startSlot, 1, &_constantBuffer);
	DEVICECTX->PSSetConstantBuffers(startSlot, 1, &_constantBuffer);
}