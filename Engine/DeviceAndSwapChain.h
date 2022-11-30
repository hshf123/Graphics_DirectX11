#pragma once

// ----------------------------
//		DeviceAndSwapChain
// ----------------------------

class DeviceAndSwapChain
{
public:
	void Init(const WindowInfo& info);

	void SetRTVDSV();
	void Present();

public:
	ID3D11Device* GetDevice() { return  _device; }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext; }
	IDXGISwapChain* GetSwapChain() { return _swapChain; }
	ID3D11RenderTargetView* GetRTV() { return  _renderTargetView; }

private:
	WindowInfo						_info;

	IDXGISwapChain*					_swapChain;
	ID3D11Device*					_device;
	ID3D11DeviceContext*			_deviceContext;
	ID3D11RenderTargetView*			_renderTargetView;
};
