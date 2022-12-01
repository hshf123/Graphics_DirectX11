#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{

}

Material::~Material()
{

}

void Material::PushGraphicsData()
{
	// CBV 업로드
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushGraphicsData(&_params, sizeof(_params));

	// SRV 업로드
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		CONTEXT->PSSetShaderResources(static_cast<uint32>(reg), 1, _textures[i]->GetSRVAddress());
		CONTEXT->PSSetSamplers(static_cast<uint32>(reg), 1, _textures[i]->GetSamplerStateAddress());
	}

	_shader->Update();
}

void Material::PushComputeData()
{
	// CBV 업로드
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushComputeData(&_params, sizeof(_params));

	// SRV 업로드
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		CONTEXT->CSSetShaderResources(static_cast<uint32>(reg), 1, _textures[i]->GetSRVAddress());
		CONTEXT->CSSetSamplers(static_cast<uint32>(reg), 1, _textures[i]->GetSamplerStateAddress());
	}

	_shader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	PushComputeData();

	CONTEXT->Dispatch(x, y, z);

}
