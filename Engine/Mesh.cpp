#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"
#include "InstancingBuffer.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{

}

Mesh::~Mesh()
{

}

// ---------------
//		Mesh
// ---------------

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	HRESULT hr;
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D11_BUFFER_DESC bd = CD3D11_BUFFER_DESC();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &buffer[0];
	hr = DEVICE->CreateBuffer(&bd, &initData, &_vertexBuffer);
	CHECK_FAIL(hr, L"Failed Create Vertex Buffer");
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	HRESULT hr;
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D11_BUFFER_DESC bd = CD3D11_BUFFER_DESC();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = &buffer[0];
	hr = DEVICE->CreateBuffer(&bd, &InitData, &_indexBuffer);
	CHECK_FAIL(hr, L"Failed Create Index Buffer");
}

void Mesh::Render(uint32 instanceCount)
{
	// Set vertex buffer
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	// Set index buffer
	CONTEXT->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->DrawIndexedInstanced(_indexCount, instanceCount, 0, 0, 0);
}

void Mesh::Render(shared_ptr<InstancingBuffer> buffer)
{
	// Set vertex buffer
	ID3D11Buffer* buffers[] = { _vertexBuffer, buffer->GetBuffer() };
	uint32 strides[] = { sizeof(Vertex), sizeof(InstancingParams) };
	uint32 offset[] = { 0,0 };
	CONTEXT->IASetVertexBuffers(0, 2, buffers, strides, offset);
	// Set index buffer
	CONTEXT->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->DrawIndexedInstanced(_indexCount, buffer->GetCount(), 0, 0, 0);
}
