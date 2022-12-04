#pragma once

#include "DeviceAndSwapChain.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RenderTargetGroup.h"

// ------------------
//		Engine
// ------------------

class Engine
{
public:
	void Init(const WindowInfo& info);
	void Update();

	void Render();

private:
	void RenderBegin();
	void RenderEnd();

public:
	void ResizeWindow(int32 width, int32 height);

	WindowInfo GetWindow() { return _info; }

	shared_ptr<DeviceAndSwapChain> GetDeviceAndSwapChain() { return _deviceAndSwapChain; }
	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return _rtGroups[static_cast<uint8>(type)]; }

private:
	void ShowFPS();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();

private:
	WindowInfo								_info;

	shared_ptr<DeviceAndSwapChain>			_deviceAndSwapChain = make_shared<DeviceAndSwapChain>();
	vector<shared_ptr<ConstantBuffer>>		_constantBuffers;
	array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;
};

