#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Light.h"
#include "Input.h"
#include "Timer.h"

// ------------------
//		Engine
// ------------------

void Engine::Init(const WindowInfo& info)
{
	_info = info;

	// 그려질 화면 크기 설정
	_viewport = { 0,0,static_cast<FLOAT>(_info.width), static_cast<FLOAT>(_info.height), 0.f, 1.f};
	_scissorRect = CD3D11_RECT(0, 0, _info.width, _info.height);

	_deviceAndSwapChain->Init(info);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256);

	CreateRenderTargetGroups();

	ResizeWindow(_info.width, _info.height);

	GET_SINGLE(Input)->Init(_info.hWnd);
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Resources)->Init();
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	Render();

	ShowFPS();
}

void Engine::Render()
{
	RenderBegin();
	
	GET_SINGLE(SceneManager)->Render();

	RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	RECT rect = { 0,0,_info.width, _info.height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_info.hWnd, 0, 100, 100, width, height, 0);
}


void Engine::RenderBegin()
{
	CONTEXT->RSSetViewports(1, &_viewport);
	ID3D11ShaderResourceView* srv[] = { nullptr,nullptr,nullptr,nullptr,nullptr };
	CONTEXT->PSSetShaderResources(0, 5, &srv[0]);
}

void Engine::RenderEnd()
{
	_deviceAndSwapChain->Present();
}

void Engine::ShowFPS()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_info.hWnd, text);
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}

void Engine::CreateRenderTargetGroups()
{
	// DepthStencil
	shared_ptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, _info.width, _info.height,
		D3D11_BIND_DEPTH_STENCIL);

	// SwapChain Group
	{
		vector<RenderTarget> rtVec(1);

		ID3D11Resource* resource;
		_deviceAndSwapChain->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&resource);
		rtVec[0].target = GET_SINGLE(Resources)->CreateTextureFromResource(L"SwapChainTarget", resource);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _info.width, _info.height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _info.width, _info.height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _info.width, _info.height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	// Lighting Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _info.width, _info.height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _info.width, _info.height,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}
}
