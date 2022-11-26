#pragma once

// ------------------
//		SwapChain
// ------------------

class SwapChain
{
public:
	void Init(const WindowInfo& info, shared_ptr<class Device> device);

	void Clear();
	
	void SetRTVDSV();
	void Present();

	IDXGISwapChain* GetSwapChain() { return _swapChain; }
	ID3D11RenderTargetView* GetRTV() { return  _renderTargetView; }

private:
	void CreateSwapChain();
	void CreateRTV();

private:
	IDXGISwapChain*				_swapChain = nullptr;
	ID3D11RenderTargetView*		_renderTargetView = nullptr;
	WindowInfo					_info;
	shared_ptr<class Device>	_device;
};

