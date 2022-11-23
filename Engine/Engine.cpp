#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

// ------------------
//		Engine
// ------------------

void Engine::Init(const WindowInfo& info)
{
	_info = info;
	ResizeWindow(_info.width, _info.height);

	// 그려질 화면 크기 설정
	_viewport = { 0,0,static_cast<FLOAT>(_info.width), static_cast<FLOAT>(_info.height), 0.f, 1.f};
	_scissorRect = CD3D11_RECT(0, 0, _info.width, _info.height);

	_device->Init();
	_swapChain->Init(_info, _device);
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);
	_dsb->Init(_info);
	GET_SINGLE(Input)->Init(_info.hWnd);
	GET_SINGLE(Timer)->Init();

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
	CONTEXT->ClearRenderTargetView(GEngine->GetSwapChain()->GetRTV(), Colors::LightSteelBlue);
	CONTEXT->ClearDepthStencilView(GEngine->GetDSB()->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
	GEngine->GetSwapChain()->SetRTVDSV();
}

void Engine::RenderEnd()
{
	_swapChain->Present();
	GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
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
