#pragma once

#include <vector>
#include <map>

#include "Module.h"
#include "ModuleFileSystem.h"

#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"

#include "External/MathGeoLib/include/Geometry/LineSegment.h"

#include "JsonFile.h"
#include "ResourceMesh.h"

class GameObject;
class CCamera;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);

	virtual ~ModuleScene();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	GameObject* CreateGameObject(std::string name, GameObject* parent);
	//void DestroyGameObject(GameObject* toDestroy);

	void ClearScene();

	void SaveScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");
	void LoadScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");
	void LoadSceneFromAssets(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");

	// Function to handle GameObject selection by Mouse Picking
	void HandleGameObjectSelection(const LineSegment& ray);

	// Function to test if picking inside an AABB
	bool IsInsideAABB(const float3& point, const AABB& aabb);

public:

	GameObject* mRootNode;

	GameObject* gameCameraObject;
	CCamera* gameCameraComponent;

	std::vector<GameObject*> gameObjects;

	JsonFile ysceneFile;

	Mesh* ourMesh;
	ResourceMesh mymesh{ 0 };

	std::string currentSceneDir;
	std::string currentSceneFile;

	std::vector<GameObject*> vSelectedGOs;
};