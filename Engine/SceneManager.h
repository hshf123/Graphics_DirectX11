#pragma once

// ----------------------
//		SceneManager
// ----------------------

class Scene;

class SceneManager
{
	SINGLETON(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

};

