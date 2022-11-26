#pragma once

// ---------------
//		Device
// ---------------

class Device
{
public:
	void Init();

	void Clear();

public:
	ID3D11Device* GetDevice() { return  _device; }
	ID3D11DeviceContext* GetDeviceContext() { return _deviceContext; }

private:
	D3D_DRIVER_TYPE				_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*				_device = nullptr;
	ID3D11DeviceContext*		_deviceContext = nullptr;
};

