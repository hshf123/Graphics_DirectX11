#include "pch.h"
#include "RenderTargetGroup.h"
#include "Engine.h"

// --------------------------
//		RenderTargetGroup
// --------------------------

void RenderTargetGroup::Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture)
{
	_groupType = groupType;
	_rtVec = rtVec;
	_rtCount = static_cast<uint32>(rtVec.size());
	_dsTexture = dsTexture;

	for (RenderTarget rt : _rtVec)
		_rtvArr.push_back(rt.target->GetRTV());
}

void RenderTargetGroup::OMSetBackBufferRenderTargets()
{
	CONTEXT->OMSetRenderTargets(1, &_rtvArr[0], _dsTexture->GetDSV());
}

void RenderTargetGroup::OMSetRenderTargets()
{
	CONTEXT->OMSetRenderTargets(_rtCount, &_rtvArr[0], _dsTexture->GetDSV());
}

void RenderTargetGroup::ClearRenderTargetView(uint32 index)
{
	CONTEXT->ClearRenderTargetView(_rtvArr[index], _rtVec[index].clearColor);
	CONTEXT->ClearDepthStencilView(_dsTexture->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void RenderTargetGroup::ClearRenderTargetView()
{
	for (uint32 i = 0; i < _rtCount; i++)
		CONTEXT->ClearRenderTargetView(_rtvArr[i], _rtVec[i].clearColor);

	CONTEXT->ClearDepthStencilView(_dsTexture->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
}