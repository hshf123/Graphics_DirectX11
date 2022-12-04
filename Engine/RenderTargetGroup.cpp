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
	D3D11_VIEWPORT vp = D3D11_VIEWPORT{ 0.f, 0.f, _rtVec[0].target->GetWidth() , _rtVec[0].target->GetHeight(), 0.f, 1.f };
	D3D11_RECT rect = D3D11_RECT{ 0, 0, static_cast<LONG>(_rtVec[0].target->GetWidth()),  static_cast<LONG>(_rtVec[0].target->GetHeight()) };

	CONTEXT->RSSetViewports(1, &vp);
	CONTEXT->RSSetScissorRects(1, &rect);

	CONTEXT->OMSetRenderTargets(1, &_rtvArr[0], _dsTexture->GetDSV());

	int8 count = static_cast<int8>(UAV_REGISTER::END);
	ID3D11UnorderedAccessView* uav = nullptr;
	for (uint8 i = static_cast<uint8>(UAV_REGISTER::u0); i < static_cast<uint8>(UAV_REGISTER::END); i++)
	{
		CONTEXT->CSSetUnorderedAccessViews(i, 1, &uav, 0);
	}
}

void RenderTargetGroup::OMSetRenderTargets()
{
	D3D11_VIEWPORT vp = D3D11_VIEWPORT{ 0.f, 0.f, _rtVec[0].target->GetWidth() , _rtVec[0].target->GetHeight(), 0.f, 1.f };
	D3D11_RECT rect = D3D11_RECT{ 0, 0, static_cast<LONG>(_rtVec[0].target->GetWidth()),  static_cast<LONG>(_rtVec[0].target->GetHeight()) };

	CONTEXT->RSSetViewports(1, &vp);
	CONTEXT->RSSetScissorRects(1, &rect);

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