#pragma once

// -----------------
//		include
// -----------------
#include <iostream>
#include <vector>
#include <fstream>

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include <DirectXTex.h>
#include <DirectXTex.inl>

#define _HAS_STD_BYTE 0
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;
using namespace DirectX;

// -----------------
//		Library
// -----------------
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// -----------------
//		typedef
// -----------------
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using Matrix = XMMATRIX;
using Vec4 = XMFLOAT4;
using Vec3 = XMFLOAT3;
using Vec2 = XMFLOAT2;
using Vec = XMVECTOR;

// -----------------
//		Global
// -----------------
#define CHECK_FAIL(hr, msg)							 \
if (FAILED(hr))										 \
{													 \
	MessageBox(nullptr, msg, L"Error", MB_OK);		 \
	return;											 \
}													 \

extern unique_ptr<class Engine> GEngine;

#define DEVICE			GEngine->GetDevice()->GetDevice()
#define DEVICECTX		GEngine->GetDevice()->GetDeviceContext()

// -----------------
//		struct
// -----------------

struct WindowInfo
{
	HWND hWnd;
	uint32 width;
	uint32 height;
	bool windowed;
};

struct Vertex
{
	Vec3 pos;
	Vec4 color;
	Vec2 uv;
};

struct Transform
{
	Vec4 offset;
};