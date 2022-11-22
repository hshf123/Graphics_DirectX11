#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"

// ------------------
//		Engine
// ------------------

class Engine
{
public:
	void Init(const WindowInfo& info);
	void Render();

	void ResizeWindow(int32 width, int32 height);

	shared_ptr<Device>		GetDevice() { return _device; }
	shared_ptr<SwapChain>	GetSwapChain() { return _swapChain; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }

private:
	WindowInfo				_info;
	D3D11_VIEWPORT			_viewport = {};
	D3D11_RECT				_scissorRect = {};

	shared_ptr<Device>		_device;
	shared_ptr<SwapChain>	_swapChain;
	shared_ptr<ConstantBuffer> _cb;
};

