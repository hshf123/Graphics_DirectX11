#pragma once

// ----------------------------
//		DeviceAndSwapChain
// ----------------------------

class DeviceAndSwapChain
{
public:
	void Init(const WindowInfo& info);

	void Present();

public:
	ID3D11Device* GetDevice() { return  _device; }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext; }
	IDXGISwapChain* GetSwapChain() { return _swapChain; }

private:
	WindowInfo						_info;

	D3D_DRIVER_TYPE					_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL				_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*					_swapChain;
	ID3D11Device*					_device;
	ID3D11DeviceContext*			_deviceContext;
};
