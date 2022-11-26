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
	HRESULT hr;
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");
	SetInputLayout();
	SetPipelineState(info);
}

void Shader::Update()
{
	CONTEXT->VSSetShader(_vertexShader, nullptr, 0);
	CONTEXT->PSSetShader(_pixelShader, nullptr, 0);
	CONTEXT->RSSetState(_rasterizer);
	CONTEXT->OMSetDepthStencilState(_depthStencilState, 0);
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
		_vsBlob->GetBufferSize(), &_vertexLayout);
	_vsBlob->Release();
	CHECK_FAIL(hr, L"Failed Create Input Layout");

	// Set the input layout
	CONTEXT->IASetInputLayout(_vertexLayout);
}

void Shader::SetPipelineState(ShaderInfo info)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	::ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc = CD3D11_RASTERIZER_DESC();

	switch (info.rasterizerType)
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

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	::ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc = CD3D11_DEPTH_STENCIL_DESC();

	switch (info.depthStencilType)
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
	
	DEVICE->CreateRasterizerState(&rasterizerDesc, &_rasterizer);
	DEVICE->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
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
