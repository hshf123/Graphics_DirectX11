#pragma once

// ---------------
//		Mesh
// ---------------

class Mesh
{
public:
	void Init(vector<Vertex>& vec);
	void Render();

	void SetTransform(const Transform& transform) { _transform = transform; }

private:
	ID3D11Buffer* _vertexBuffer = nullptr;
	uint32 _vertexCount = 0;

	Transform _transform = {};
};

