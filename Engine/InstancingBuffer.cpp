#include "pch.h"
#include "InstancingBuffer.h"
#include "Engine.h"

// -------------------------
//		InstancingBuffer
// -------------------------

InstancingBuffer::InstancingBuffer()
{
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::Init(uint32 maxCount)
{
	_maxCount = maxCount;

	const int32 bufferSize = sizeof(InstancingParams) * maxCount;

	D3D11_BUFFER_DESC bd = CD3D11_BUFFER_DESC();
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	HRESULT hr = DEVICE->CreateBuffer(&bd, nullptr, &_buffer);
	CHECK_FAIL(hr, L"Failed Create Instancing Buffer");
}

void InstancingBuffer::Clear()
{
	_data.clear();
}

void InstancingBuffer::AddData(InstancingParams& params)
{
	_data.push_back(params);
}

void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount)
		Init(dataCount);

	const uint32 bufferSize = dataCount * sizeof(InstancingParams);
	CONTEXT->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_subresource);
	memcpy(_subresource.pData, &_data[0], bufferSize);
	CONTEXT->Unmap(_buffer, 0);
}