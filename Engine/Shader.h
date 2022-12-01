#pragma once
#include "Object.h"

// -----------------
//		Shader
// -----------------

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	COMPUTE,
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
	NO_DEPTH_TEST,				// 깊이 테스트(X) + 깊이 기록(0)
	NO_DEPTH_TEST_NO_WRITE,		// 깊이 테스트(X) + 깊이 기록(X)
	LESS_NO_WRITE,				// 깊이 테스트(0) + 깊이 기록(X)
};

enum class BLEND_TYPE : uint8
{
	DEFAULT,			// 덮어쓰기
	ALPHA_BLEND,		// ALPHA(투명도)를 이용한 블렌딩
	ONE_TO_ONE_BLEND,	// 1:1 비율로
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void CreateGraphicsShader(const WCHAR* path, ShaderInfo info = ShaderInfo(), LPCSTR vs = "VS_Main", LPCSTR ps = "PS_Main");
	void CreateComputeShader(const WCHAR* path, LPCSTR main, LPCSTR version);
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

private:

	ID3DBlob* _csBlob = nullptr;
	ID3D11ComputeShader* _computeShader;
};

