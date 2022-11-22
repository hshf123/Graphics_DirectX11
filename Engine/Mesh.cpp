#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

// ---------------
//		Mesh
// ---------------

void Mesh::Init(vector<Vertex>& vec)
{
	HRESULT hr;
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = &vec[0];
	hr = DEVICE->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	CHECK_FAIL(hr, L"Failed Create Vertex Buffer");
}

void Mesh::Render()
{
	// Set vertex buffer
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;
	DEVICECTX->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	// Set primitive topology
	DEVICECTX->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GEngine->GetCB()->PushData(0, &_transform);
	GEngine->GetCB()->PushData(1, &_transform);

	DEVICECTX->Draw(_vertexCount, 0);
}
