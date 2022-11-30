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
	ID3D11ShaderResourceView* GetSRV() { return _srv; }
	ID3D11ShaderResourceView** GetSRVAddress() { return &_srv; }
	ID3D11SamplerState** GetSamplerStateAddress() { return &_samplerState; }

private:


private:
	ScratchImage			 		_image;
	ID3D11Texture2D*				_tex2D;

	ID3D11RenderTargetView*			_rtv;
	ID3D11DepthStencilView*			_dsv;
	ID3D11ShaderResourceView*		_srv;
	ID3D11SamplerState*				_samplerState;
};

