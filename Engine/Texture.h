#pragma once

// -----------------
//		Texture
// -----------------

class Texture
{
public:
	void Init(const wstring& path);

	void Render();

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

