#pragma once
#include "Object.h"

// -----------------
//		Texture
// -----------------

class Texture :public Object
{
public:
	Texture();
	virtual ~Texture();

	virtual void Load(const wstring& path) override;

public:
	void Create(DXGI_FORMAT format, uint32 width, uint32 height, uint32 bindFlags);
	void CreateFromResource(ID3D11Resource* tex2D);

public:
	ID3D11RenderTargetView* GetRTV() { return _rtv; }
	ID3D11DepthStencilView* GetDSV() { return _dsv; }
	ID3D11ShaderResourceView** GetSRVAddress() { return &_srv; }
	ID3D11UnorderedAccessView** GetUAVAddress() { return &_uav; }
	ID3D11SamplerState** GetSamplerStateAddress() { return &_samplerState; }

	float GetWidth() { return _desc.Width; }
	float GetHeight() { return _desc.Height; }

private:
	ScratchImage			 		_image;
	D3D11_TEXTURE2D_DESC			_desc;
	ID3D11Texture2D*				_tex2D;

	ID3D11RenderTargetView*			_rtv;
	ID3D11DepthStencilView*			_dsv;
	ID3D11ShaderResourceView*		_srv;
	ID3D11UnorderedAccessView*		_uav;

	ID3D11SamplerState*				_samplerState;
};

