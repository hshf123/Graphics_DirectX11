#pragma once

// ------------------------
//		ConstantBuffer
// ------------------------

enum class CONSTANT_BUFFER_TYPE : uint8
{
	TRANSFORM,
	MATERIAL,
	END
};

class ConstantBuffer
{
public:
	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void PushData(void* buffer);

private:
	ID3D11Buffer* _constantBuffer = nullptr;

	CBV_REGISTER _reg = {};
};

