#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "TestCameraScript.h"
#include "Resources.h"

// ----------------------
//		SceneManager
// ----------------------

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	 map<wstring, uint8>::iterator findIt = _layerIndex.find(name);
	 if (findIt == _layerIndex.end())
		 return 0;

	 return findIt->second;
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안찍겠다.
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끈다.
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍겠다.
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->SetName(L"SkyBox");
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Skybox"));
			material->SetTexture(0, GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg"));
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Cube
	{
		shared_ptr<GameObject> cube = make_shared<GameObject>();
		cube->SetName(L"Cube");
		cube->AddComponent(make_shared<Transform>());
		cube->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		cube->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));
			material->SetTexture(0, GET_SINGLE(Resources)->Load<Texture>(L"Stone", L"..\\Resources\\Texture\\Stone.jpg"));
			material->SetTexture(1, GET_SINGLE(Resources)->Load<Texture>(L"Stone_Normal", L"..\\Resources\\Texture\\Stone_Normal.jpg"));
			meshRenderer->SetMaterial(material);
		}
		cube->AddComponent(meshRenderer);
		scene->AddGameObject(cube);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 3; ++i)
	{
		shared_ptr<GameObject> rectangle = make_shared<GameObject>();
		rectangle->SetName(L"Rectangle");
		rectangle->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
		rectangle->AddComponent(make_shared<Transform>());
		rectangle->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		rectangle->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 160), 250.f, 500.f)); // z값은 딱히 의미없음(0~1000)
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Forward"));
			material->SetTexture(0, GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i));
			meshRenderer->SetMaterial(material);
		}
		rectangle->AddComponent(meshRenderer);
		scene->AddGameObject(rectangle);
	}
#pragma endregion

#pragma region White Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.f, 0.1f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));

		scene->AddGameObject(light);
	}
#pragma endregion

	return scene;
}