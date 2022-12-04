#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"
#include "Material.h"
#include "Light.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{
	
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
	if (_reg == CBV_REGISTER::b0)
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = DEVICE->CreateBuffer(&bd, nullptr, &_constantBuffer);
	CHECK_FAIL(hr, L"Failed Create Constant Buffer");
}

void ConstantBuffer::Clear()
{
	if (_constantBuffer) _constantBuffer = nullptr;
}

void ConstantBuffer::PushGraphicsData(void* buffer, uint32 size)
{
	assert(_elementSize == ((size + 255) & ~255));

	uint32 slot = static_cast<uint32>(_reg);
	CONTEXT->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedBuffer);
	::memcpy(_mappedBuffer.pData, buffer, size);
	CONTEXT->Unmap(_constantBuffer, 0);
	CONTEXT->VSSetConstantBuffers(slot, 1, &_constantBuffer);
	CONTEXT->GSSetConstantBuffers(slot, 1, &_constantBuffer);
	CONTEXT->PSSetConstantBuffers(slot, 1, &_constantBuffer);
}

void ConstantBuffer::SetGraphicsGlobalData(void* buffer, uint32 size)
{
	assert(_reg == CBV_REGISTER::b0);
	assert(_elementSize == ((size + 255) & ~255));

	uint32 slot = static_cast<uint32>(_reg);
	CONTEXT->UpdateSubresource(_constantBuffer, 0, nullptr, buffer, 0, 0);
	CONTEXT->VSSetConstantBuffers(slot, 1, &_constantBuffer);
	CONTEXT->GSSetConstantBuffers(slot, 1, &_constantBuffer);
	CONTEXT->PSSetConstantBuffers(slot, 1, &_constantBuffer);
}

void ConstantBuffer::PushComputeData(void* buffer, uint32 size)
{
	assert(_elementSize == ((size + 255) & ~255));

	uint32 slot = static_cast<uint32>(_reg);
	CONTEXT->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedBuffer);
	::memcpy(_mappedBuffer.pData, buffer, size);
	CONTEXT->Unmap(_constantBuffer, 0);
	CONTEXT->CSSetConstantBuffers(slot, 1, &_constantBuffer);
}
