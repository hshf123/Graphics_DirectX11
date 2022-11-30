#pragma once
#include "Texture.h"

// --------------------------
//		RenderTargetGroup
// --------------------------

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN,		// BACK_BUFFER
	G_BUFFER,		// POSITION, NORMAL, COLOR
	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END),
};

struct RenderTarget
{
	shared_ptr<Texture> target;
	float clearColor[4];
};

class RenderTargetGroup
{
public:
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);

	void OMSetBackBufferRenderTargets();
	void OMSetRenderTargets();

	void ClearRenderTargetView(uint32 index);
	void ClearRenderTargetView();

	shared_ptr<Texture> GetRTTexture(uint32 index) { return _rtVec[index].target; }
	shared_ptr<Texture> GetDSTexture() { return _dsTexture; }

private:
	RENDER_TARGET_GROUP_TYPE		_groupType;
	vector<RenderTarget>			_rtVec;
	uint32							_rtCount;
	shared_ptr<Texture>				_dsTexture;
	vector<ID3D11RenderTargetView*> _rtvArr;
};

