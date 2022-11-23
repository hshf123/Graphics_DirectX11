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
			CONTEXT->Unmap(_constantBuffer, 0);

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
}

void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	assert(_elementSize == ((size + 255) & ~255));

	CONTEXT->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedBuffer);
	uint32 slot = static_cast<uint32>(_reg);
	BYTE* data = static_cast<BYTE*>(_mappedBuffer.pData);
	::memcpy(_mappedBuffer.pData, buffer, size);
	CONTEXT->Unmap(_constantBuffer, 0);
	CONTEXT->VSSetConstantBuffers(slot, 1, &_constantBuffer);
	CONTEXT->PSSetConstantBuffers(slot, 1, &_constantBuffer);
}