#pragma once

// --------------------------
//		DepthStencilBuffer
// --------------------------

class DepthStencilBuffer
{
public:
	void Init(const WindowInfo& info);
	void Clear();

	ID3D11DepthStencilView* GetDSV() { return _depthStencilView; }

private:

	ID3D11Texture2D*			_depthStencilTexture = nullptr;
	ID3D11DepthStencilView*		_depthStencilView = nullptr;
};

