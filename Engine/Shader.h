#pragma once
#include "Object.h"

// -----------------
//		Shader
// -----------------

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
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

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

private:
	void CreateVertexShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);
	void CreatePixelShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version);
	void SetInputLayout();
	void SetRasterizerState();
	void SetDepthStencilState();
	void SetBlendState();

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

private:
	ShaderInfo					_info;

	ID3DBlob*					_vsBlob = nullptr;
	ID3DBlob*					_psBlob = nullptr;

	ID3D11VertexShader*			_vertexShader = nullptr;
	ID3D11PixelShader*			_pixelShader = nullptr;
	ID3D11InputLayout*			_inputLayout = nullptr;

	ID3D11RasterizerState*		_rasterizer;
	ID3D11DepthStencilState*	_depthStencilState;
	ID3D11BlendState*			_blendState;
};

