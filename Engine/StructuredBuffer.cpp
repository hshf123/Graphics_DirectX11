#include "pch.h"
#include "StructuredBuffer.h"
#include "Engine.h"

// ------------------------
//		StructedBuffer
// ------------------------

StructuredBuffer::StructuredBuffer()
{
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::Init(uint32 elementSize, uint32 elementCount)
{
	_elementSize = elementSize;
	_elementCount = elementCount;

	// Buffer
	{
		uint64 bufferSize = static_cast<uint64>(_elementSize) * _elementCount;
		D3D11_BUFFER_DESC bd = CD3D11_BUFFER_DESC();
		bd.ByteWidth = bufferSize;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = _elementSize;
		bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		HRESULT hr = DEVICE->CreateBuffer(&bd, nullptr, (ID3D11Buffer**)&_structedBuffer);
		CHECK_FAIL(hr, L"Failed Create Structured Buffer");
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
		// Setup the description of the shader resource view.
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = _elementCount;
		// Create the shader resource view.
		HRESULT hr = DEVICE->CreateShaderResourceView(_structedBuffer, &srvDesc, &_srv);
		CHECK_FAIL(hr, L"Failed to Create ShaderResourceView");

		D3D11_SAMPLER_DESC sampDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = DEVICE->CreateSamplerState(&sampDesc, &_samplerState);
		CHECK_FAIL(hr, L"Failed to Create SamplerState");
	}

	// UAV
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = CD3D11_UNORDERED_ACCESS_VIEW_DESC();
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = _elementCount;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;

		HRESULT hr = DEVICE->CreateUnorderedAccessView(_structedBuffer, &uavDesc, &_uav);
		CHECK_FAIL(hr, L"Failed Create Unordered Access View");
	}
}

void StructuredBuffer::PushGraphicsData(SRV_REGISTER reg)
{
	uint32 startSlot = static_cast<uint32>(reg);
	CONTEXT->VSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->VSSetSamplers(startSlot, 1, &_samplerState);
	CONTEXT->HSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->HSSetSamplers(startSlot, 1, &_samplerState);
	CONTEXT->DSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->DSSetSamplers(startSlot, 1, &_samplerState);
	CONTEXT->GSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->GSSetSamplers(startSlot, 1, &_samplerState);
	CONTEXT->PSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->PSSetSamplers(startSlot, 1, &_samplerState);
}

void StructuredBuffer::PushComputeSRVData(SRV_REGISTER reg)
{
	uint32 startSlot = static_cast<uint32>(reg);
	CONTEXT->CSSetShaderResources(startSlot, 1, &_srv);
	CONTEXT->CSSetSamplers(startSlot, 1, &_samplerState);
}

void StructuredBuffer::PushComputeUAVData(UAV_REGISTER reg)
{
	uint32 startSlot = static_cast<uint32>(reg);
	CONTEXT->CSSetUnorderedAccessViews(startSlot, 1, &_uav, nullptr);
}
