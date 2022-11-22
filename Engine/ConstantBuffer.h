#pragma once

// ------------------------
//		ConstantBuffer
// ------------------------

class ConstantBuffer
{
public:
	void Init(uint32 size);

	void PushData(uint32 startSlot, void* buffer);

private:
	ID3D11Buffer* _constantBuffer = nullptr;
};

