#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::Update()
{
	// CBV ���ε�
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params);

	// SRV ���ε�
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		DEVICECTX->PSSetShaderResources(static_cast<uint32>(reg), 1, _textures[i]->GetSRVAddress());
		DEVICECTX->PSSetSamplers(static_cast<uint32>(reg), 1, _textures[i]->GetSamplerStateAddress());
	}

	// ���������� ����
	_shader->Update();
}