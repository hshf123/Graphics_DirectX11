#pragma once

// ---------------
//		Mesh
// ---------------

class Mesh
{
public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);
	void Render();

	void SetTransform(const Transform& transform) { _transform = transform; }
	void SetTexture(shared_ptr<class Texture> tex) { _tex = tex; }

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

private:
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;
	uint32 _vertexCount = 0;
	uint32 _indexCount = 0;

	Transform _transform = {};
	shared_ptr<class Texture> _tex = {};
};

