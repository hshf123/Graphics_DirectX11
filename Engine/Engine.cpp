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

	_dsb->Init(_info);
	GET_SINGLE(Input)->Init(_info.hWnd);
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Resources)->Init();

	ResizeWindow(_info.width, _info.height);
	GEngine->GetDeviceAndSwapChain()->SetRTVDSV();
	CONTEXT->RSSetViewports(1, &_viewport);
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
	CONTEXT->ClearRenderTargetView(GEngine->GetDeviceAndSwapChain()->GetRTV(), Colors::Black);
	CONTEXT->ClearDepthStencilView(GEngine->GetDSB()->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
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
