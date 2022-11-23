#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

// ------------------------
//		ConstantBuffer
// ------------------------

void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	_reg = reg;

	uint32 elementSize = (size + 255) & ~255;
	uint32 elementCount = count;
	uint32 bufferSize = elementSize * elementCount;

	HRESULT hr;
	D3D11_BUFFER_DESC bd = {};
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = DEVICE->CreateBuffer(&bd, nullptr, &_constantBuffer);
	CHECK_FAIL(hr, L"Failed Create Constant Buffer");
}

void ConstantBuffer::PushData(void* buffer)
{
	uint32 slot = static_cast<uint32>(_reg);
	DEVICECTX->UpdateSubresource(_constantBuffer, 0, nullptr, buffer, 0, 0);
	DEVICECTX->VSSetConstantBuffers(slot, 1, &_constantBuffer);
	DEVICECTX->PSSetConstantBuffers(slot, 1, &_constantBuffer);
}