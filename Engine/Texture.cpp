#include "pch.h"
#include "Texture.h"
#include "Engine.h"

// -----------------
//		Texture
// -----------------

void Texture::Init(const wstring& path)
{
	CreateTexture(path);
	CreateView();
	CreateSamplerState();
}

void Texture::Render()
{
	DEVICECTX->PSSetShaderResources(0, 1, &_srv);
	DEVICECTX->PSSetSamplers(0, 1, &_samplerState);
}

void Texture::CreateTexture(const wstring& path)
{
	// ���� Ȯ���� ���
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, _image);
	else // png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

	HRESULT hr = ::CreateTexture(DEVICE, _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(), (ID3D11Resource**)&_tex2D);
	CHECK_FAIL(hr, L"Failed to Create Texture");
}

void Texture::CreateView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D11_TEXTURE2D_DESC textureDesc;
	_tex2D->GetDesc(&textureDesc);

	srvDesc.Format = textureDesc.Format;

	HRESULT hr = DEVICE->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
	CHECK_FAIL(hr, L"Failed to Create ShaderResourceView");
}

void Texture::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	HRESULT hr = DEVICE->CreateSamplerState(&samplerDesc, &_samplerState);
	CHECK_FAIL(hr, L"Failed to Create SamplerState");
}