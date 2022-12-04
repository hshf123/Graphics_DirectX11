#pragma once

// ------------------------
//		StructedBuffer
// ------------------------

struct InstancingParams
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
};

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

	void Init(uint32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	uint32						GetCount() { return static_cast<uint32>(_data.size()); }
	ID3D11Buffer*				GetBuffer() { return _buffer; }

	void	SetID(uint64 instanceId) { _instanceId = instanceId; }
	uint64	GetID() { return _instanceId; }

private:
	uint64						_instanceId = 0;
	ID3D11Buffer*				_buffer;
	D3D11_MAPPED_SUBRESOURCE _subresource;

	uint32						_maxCount = 0;
	vector<InstancingParams>	_data;
};