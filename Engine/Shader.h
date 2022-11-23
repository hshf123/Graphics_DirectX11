#pragma once
#include "Object.h"

// -----------------
//		Shader
// -----------------

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const WCHAR* path);
	void Update();

private:
	void CreateVertexShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);
	void CreatePixelShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	ID3DBlob*					_vsBlob = nullptr;
	ID3DBlob*					_psBlob = nullptr;

	ID3D11VertexShader*			_vertexShader = nullptr;
	ID3D11PixelShader*			_pixelShader = nullptr;
	ID3D11InputLayout*			_vertexLayout = nullptr;
};

