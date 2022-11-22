#include "pch.h"
#include "Shader.h"
#include "Engine.h"

// -----------------
//		Shader
// -----------------

void Shader::Init(const WCHAR* path)
{
	HRESULT hr;
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	uint32 numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = DEVICE->CreateInputLayout(layout, numElements, _vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), &_vertexLayout);
	_vsBlob->Release();
	CHECK_FAIL(hr, L"Failed Create Input Layout");
}

void Shader::Update()
{
	// Set the input layout
	DEVICECTX->IASetInputLayout(_vertexLayout);
	// Render a triangle
	DEVICECTX->VSSetShader(_vertexShader, nullptr, 0);
	DEVICECTX->PSSetShader(_pixelShader, nullptr, 0);
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
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
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
