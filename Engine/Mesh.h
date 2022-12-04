#pragma once
#include "Object.h"

// ---------------
//		Mesh
// ---------------

class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);
	void Render(uint32 instanceCount = 1);
	void Render(shared_ptr<class InstancingBuffer> buffer);

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

private:
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;
	uint32 _vertexCount = 0;
	uint32 _indexCount = 0;
};

