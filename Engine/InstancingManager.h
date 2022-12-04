#pragma once
#include "InstancingBuffer.h"

// --------------------------
//		InstancingManager
// --------------------------

class GameObject;

class InstancingManager
{
	SINGLETON(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

private:
	void AddParam(uint64 instanceId, InstancingParams& data);

private:
	map<uint64/*instanceId*/, shared_ptr<InstancingBuffer>> _buffers;
};
