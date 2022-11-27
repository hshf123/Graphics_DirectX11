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

	ID3D11ShaderResourceView** GetSRVAddress() { return &_srv; }
	ID3D11SamplerState** GetSamplerStateAddress() { return &_samplerState; }

private:
	void CreateTexture(const wstring& path);
	void CreateView();
	void CreateSamplerState();

private:
	ScratchImage			 		_image;
	ID3D11Texture2D*				_tex2D;
	ID3D11ShaderResourceView*		_srv;
	ID3D11SamplerState*				_samplerState;
};

