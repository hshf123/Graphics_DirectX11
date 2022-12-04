#pragma once

// ------------------------
//		StructedBuffer
// ------------------------

class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

	void Init(uint32 elementSize, uint32 elementCount);

	void PushGraphicsData(SRV_REGISTER reg);
	void PushComputeSRVData(SRV_REGISTER reg);
	void PushComputeUAVData(UAV_REGISTER reg);

	ID3D11Resource* GetBuffer() { return _structedBuffer; }

private:
	ID3D11Resource*					_structedBuffer = nullptr;

	uint32							_elementSize = 0;
	uint32							_elementCount = 0;

	ID3D11ShaderResourceView*		_srv = nullptr;
	ID3D11SamplerState*				_samplerState = nullptr;
	ID3D11UnorderedAccessView*		_uav = nullptr;
};
