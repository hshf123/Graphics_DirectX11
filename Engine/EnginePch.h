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

using namespace std;
using namespace DirectX;

// -----------------
//		Library
// -----------------
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")

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
