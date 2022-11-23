#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

#include "Input.h"
#include "Timer.h"

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
	shared_ptr<DepthStencilBuffer> GetDSB() { return _dsb; }
	shared_ptr<Input> GetInput() { return _input; }
	shared_ptr<Timer> GetTimer() { return _timer; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

public:
	void Update();
	void LateUpdate();

private:
	void ShowFPS();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);

private:
	WindowInfo								_info;
	D3D11_VIEWPORT							_viewport = {};
	D3D11_RECT								_scissorRect = {};

	shared_ptr<Device>						_device = make_shared<Device>();
	shared_ptr<SwapChain>					_swapChain = make_shared<SwapChain>();
	vector<shared_ptr<ConstantBuffer>>		_constantBuffers;
	shared_ptr<DepthStencilBuffer>			_dsb = make_shared<DepthStencilBuffer>();

	shared_ptr<Input>						_input = make_shared<Input>();
	shared_ptr<Timer>						_timer = make_shared<Timer>();
};

