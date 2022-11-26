#pragma once

// ------------------------
//		ConstantBuffer
// ------------------------

enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void PushData(void* buffer, uint32 size);

	void SetGlobalData(void* buffer, uint32 size);

private:
	ID3D11Buffer* _constantBuffer = nullptr;
	D3D11_MAPPED_SUBRESOURCE _mappedBuffer;

	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	CBV_REGISTER _reg = {};
};

