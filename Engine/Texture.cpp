#include "pch.h"
#include "Texture.h"
#include "Engine.h"

// -----------------
//		Texture
// -----------------

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{

}

Texture::~Texture()
{

}

void Texture::Load(const wstring& path)
{
	// 파일 확장자 얻기
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, _image);
	else // png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

	HRESULT hr = ::CreateTexture(DEVICE, _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(), (ID3D11Resource**)&_tex2D);
	CHECK_FAIL(hr, L"Failed to Create Texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D11_TEXTURE2D_DESC textureDesc = CD3D11_TEXTURE2D_DESC();
	_tex2D->GetDesc(&textureDesc);

	srvDesc.Format = textureDesc.Format;

	hr = DEVICE->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
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

void Texture::Create(DXGI_FORMAT format, uint32 width, uint32 height, uint32 bindFlags)
{
	HRESULT hr;
	
	D3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC();
	if (bindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		// Create depth stencil texture
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
	}
	else if (bindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		// Setup the render target texture description.
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
	}

	// Create Texture2D
	hr = DEVICE->CreateTexture2D(&desc, nullptr, &_tex2D);
	CHECK_FAIL(hr, L"Failed Create Depth Stencil Texture");

	assert(SUCCEEDED(hr));

	CreateFromResource(_tex2D);
}

void Texture::CreateFromResource(ID3D11Resource* tex2D)
{
	HRESULT hr;

	_tex2D = static_cast<ID3D11Texture2D*>(tex2D);

	D3D11_TEXTURE2D_DESC desc = CD3D11_TEXTURE2D_DESC();
	_tex2D->GetDesc(&desc);

	// 주요 조합
	// - DSV 단독 (조합X)
	// - SRV
	// - RTV + SRV
	if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		// DSV
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
		dsvDesc.Format = desc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		hr = DEVICE->CreateDepthStencilView(_tex2D, &dsvDesc, &_dsv);
		CHECK_FAIL(hr, L"Failed Create Depth Stencil View");
	}
	else
	{
		if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			// RTV
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
			rtvDesc.Format = desc.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
			// Create the render target view.
			hr = DEVICE->CreateRenderTargetView(_tex2D, &rtvDesc, &_rtv);
			CHECK_FAIL(hr, L"Failed Create Render Target View");
		}
		if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			// SRV
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
			// Setup the description of the shader resource view.
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			// Create the shader resource view.
			hr = DEVICE->CreateShaderResourceView(_tex2D, &srvDesc, &_srv);
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
	}
}