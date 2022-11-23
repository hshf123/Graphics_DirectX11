#include "pch.h"
#include "Engine.h"
#include "Material.h"

// ------------------
//		Engine
// ------------------

void Engine::Init(const WindowInfo& info)
{
	_info = info;
	ResizeWindow(_info.width, _info.height);

	// �׷��� ȭ�� ũ�� ����
	_viewport = { 0,0,static_cast<FLOAT>(_info.width), static_cast<FLOAT>(_info.height), 0.f, 1.f};
	_scissorRect = CD3D11_RECT(0, 0, _info.width, _info.height);

	_device->Init();
	_swapChain->Init(_info, _device);
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(Transform), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);
	_dsb->Init(_info);
	_input->Init(_info.hWnd);
	_timer->Init();

	DEVICECTX->RSSetViewports(1, &_viewport);
}

void Engine::Render()
{
	// Render Begin
	DEVICECTX->ClearRenderTargetView(GEngine->GetSwapChain()->GetRTV(), Colors::LightSteelBlue);
	DEVICECTX->ClearDepthStencilView(GEngine->GetDSB()->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
	GEngine->GetSwapChain()->SetRTVDSV();

	// TODO

	// Render End
	_swapChain->Present();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	RECT rect = { 0,0,_info.width, _info.height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_info.hWnd, 0, 100, 100, width, height, 0);
}

void Engine::Update()
{
	_timer->Update();
	_input->Update();

	ShowFPS();
}

void Engine::ShowFPS()
{
	uint32 fps = _timer->GetFps();

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
