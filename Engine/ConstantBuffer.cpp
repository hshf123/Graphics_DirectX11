#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{
	if (_constantBuffer)
	{
		if (_constantBuffer != nullptr)
			DEVICECTX->Unmap(_constantBuffer, 0);

		_constantBuffer = nullptr;
	}
}

// ------------------------
//		ConstantBuffer
// ------------------------

void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	_reg = reg;

	_elementSize = (size + 255) & ~255;
	_elementCount = count;
	uint32 bufferSize = _elementSize * _elementCount;

	HRESULT hr;
	D3D11_BUFFER_DESC bd = {};
	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = DEVICE->CreateBuffer(&bd, nullptr, &_constantBuffer);
	CHECK_FAIL(hr, L"Failed Create Constant Buffer");

	DEVICECTX->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedBuffer);
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	uint32 slot = static_cast<uint32>(_reg);
	BYTE* data = (BYTE*)_mappedBuffer.pData;
    ::memcpy(&data[_currentIndex * _elementSize], buffer, size);
	DEVICECTX->VSSetConstantBuffers(slot, 1, &_constantBuffer);
	DEVICECTX->PSSetConstantBuffers(slot, 1, &_constantBuffer);
	_currentIndex++;
}