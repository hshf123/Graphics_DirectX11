#pragma once

// -----------------
//		include
// -----------------
#include <iostream>
#include <vector>
#include <array>
#include <fstream>

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <DirectXPackedVector.h>

#include "SimpleMath.h"
#include "SimpleMath.inl"
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
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec4		= DirectX::SimpleMath::Vector4;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec2		= DirectX::SimpleMath::Vector2;
using Matrix	= DirectX::SimpleMath::Matrix;

// -----------------
//		Global
// -----------------
#define CHECK_FAIL(hr, msg)							 \
if (FAILED(hr))										 \
{													 \
	MessageBox(nullptr, msg, L"Error", MB_OK);		 \
	return;											 \
}													 \

#define SINGLETON(type)								 \
private:											 \
	type() { }								 		 \
	~type() { }								 		 \
													 \
public:												 \
	static type* GetInstance()				 		 \
	{												 \
		static type instance;				 		 \
		return &instance;							 \
	}												 \

#define GET_SINGLE(type)		type::GetInstance()

extern unique_ptr<class Engine> GEngine;

#define DEVICE				GEngine->GetDevice()->GetDevice()
#define CONTEXT			GEngine->GetDevice()->GetDeviceContext()
#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

#define INPUT				GET_SINGLE(Input)
#define DELTATIME			GET_SINGLE(Timer)->GetDeltaTime()

// -----------------
//		struct
// -----------------

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0,
	t1,
	t2,
	t3,
	t4,

	END
};

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

struct TransformParams
{
	Matrix matWVP;
};