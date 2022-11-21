#include "pch.h"
#include "Engine.h"

// ------------------
//		Engine
// ------------------

void Engine::Init(const WindowInfo& info)
{
	_info = info;
	ResizeWindow(_info.width, _info.height);

	// 그려질 화면 크기 설정
	_viewport = { 0,0,static_cast<FLOAT>(_info.width), static_cast<FLOAT>(_info.height), 0.f,1.f };
	_scissorRect = CD3D11_RECT(0, 0, _info.width, _info.height);

	_device = make_shared<Device>();
	_swapChain = make_shared<SwapChain>();

	_device->Init();
	_swapChain->Init(_info, _device);

	DEVICECTX->RSSetViewports(1, &_viewport);
}

void Engine::Render()
{
	// Just clear the backbuffer, Render Begine
	_device->GetDeviceContext()->ClearRenderTargetView(_swapChain->GetRTV(), Colors::LightSteelBlue);

	_swapChain->Present();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	RECT rect = { 0,0,_info.width, _info.height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_info.hWnd, 0, 100, 100, width, height, 0);
}
