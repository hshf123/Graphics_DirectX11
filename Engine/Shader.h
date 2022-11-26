#pragma once
#include "Object.h"

// -----------------
//		Shader
// -----------------

enum class RASTERIZER_TYPE
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const WCHAR* path, ShaderInfo info = ShaderInfo());
	void Update();

private:
	void CreateVertexShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);
	void CreatePixelShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);
	void SetInputLayout();
	void SetPipelineState(ShaderInfo info);

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	ID3DBlob*					_vsBlob = nullptr;
	ID3DBlob*					_psBlob = nullptr;

	ID3D11VertexShader*			_vertexShader = nullptr;
	ID3D11PixelShader*			_pixelShader = nullptr;
	ID3D11InputLayout*			_vertexLayout = nullptr;

	ID3D11RasterizerState*		_rasterizer;
	ID3D11DepthStencilState*	_depthStencilState;
};

