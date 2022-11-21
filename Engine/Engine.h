#pragma once

// ------------------
//		Engine
// ------------------

class Engine
{
public:
	void Init(const WindowInfo& info);
	void Render();

	void ResizeWindow(int32 width, int32 height);
private:
	WindowInfo		_info;
	D3D11_VIEWPORT	_viewport = {};
	D3D11_RECT		_scissorRect = {};

	shared_ptr<class Device> _device;
	shared_ptr<class SwapChain> _swapChain;
};

