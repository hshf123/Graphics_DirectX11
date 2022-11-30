#include "pch.h"
#include "Shader.h"
#include "Engine.h"

// -----------------
//		Shader
// -----------------

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{

}

Shader::~Shader()
{

}

void Shader::Init(const WCHAR* path, ShaderInfo info)
{
	_info = info;
	HRESULT hr;
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");
	SetInputLayout();
	SetRasterizerState();
	SetDepthStencilState();
	SetBlendState();
}

void Shader::Update()
{
	float blendFactor[4];
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	// Set the input layout
	CONTEXT->IASetInputLayout(_inputLayout);
	CONTEXT->VSSetShader(_vertexShader, nullptr, 0);
	CONTEXT->PSSetShader(_pixelShader, nullptr, 0);
	CONTEXT->RSSetState(_rasterizer);
	CONTEXT->OMSetDepthStencilState(_depthStencilState, 0);
	CONTEXT->OMSetBlendState(_blendState, blendFactor, 0xffffffff);
}

void Shader::CreateVertexShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version)
{
	HRESULT hr;
	// Compile the vertex shader
	hr = CompileShaderFromFile(path, mainFunc, version, &_vsBlob);
	CHECK_FAIL(hr, L"Failed Compile Vertex Shader");

	// Create the vertex shader
	hr = DEVICE->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, &_vertexShader);
	CHECK_FAIL(hr, L"Failed Create Vertex Shader");
}

void Shader::CreatePixelShader(const WCHAR* path, LPCSTR mainFunc, LPCSTR version)
{
	HRESULT hr;
	// Compile the pixel shader
	hr = CompileShaderFromFile(path, mainFunc, version, &_psBlob);
	CHECK_FAIL(hr, L"Failed Compile Pixel Shader");

	// Create the pixel shader
	hr = DEVICE->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, &_pixelShader);
	_psBlob->Release();
	CHECK_FAIL(hr, L"Failed Create Pixel Shader");
}

void Shader::SetInputLayout()
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	uint32 numElements = ARRAYSIZE(layout);

	// Create the input layout
	HRESULT hr = DEVICE->CreateInputLayout(layout, numElements, _vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), &_inputLayout);
	_vsBlob->Release();
	CHECK_FAIL(hr, L"Failed Create Input Layout");
}

void Shader::SetRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(D3D11_DEFAULT);

	switch (_info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		break;
	}
	
	DEVICE->CreateRasterizerState(&rasterizerDesc, &_rasterizer);
}

void Shader::SetDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	::ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);

	switch (_info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	}

	DEVICE->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
}

void Shader::SetBlendState()
{
	D3D11_BLEND_DESC bsDesc = CD3D11_BLEND_DESC(D3D11_DEFAULT);
	// Clear the blend state description.
	//ZeroMemory(&bsDesc, sizeof(D3D11_BLEND_DESC));
	//bsDesc.RenderTarget[0].BlendEnable = TRUE;
	//bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//bsDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	// Create the blend state using the description.
	HRESULT hr = DEVICE->CreateBlendState(&bsDesc, &_blendState);
	CHECK_FAIL(hr, L"Failed to Create Blend State");
}

HRESULT Shader::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = ::D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
