#pragma once

#include <vector>
#include <map>

#include "Module.h"
#include "ModuleFileSystem.h"
#include "ModuleMonoManager.h"

#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"

#include "External/MathGeoLib/include/Geometry/LineSegment.h"

#include "CAudioListener.h"
#include "CAudioSource.h"

#include "JsonFile.h"
#include "ResourceMesh.h"

#include "UI_Image.h"

#include <map>

class GameObject;
class CCamera;

class G_UI;

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
	//TODO:
	GameObject* PostUpdateCreateGameObject(std::string name, GameObject* parent);

	G_UI* CreateGUI(UI_TYPE t, GameObject* pParent = nullptr, int x = 0, int y = 0);
	//void DestroyGameObject(GameObject* toDestroy);

	void ClearScene();

	void SaveScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");
	void LoadScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");

	// Start with a loaded scene from start
	void LoadSceneFromStart(const std::string& dir, const std::string& fileName);

	void Destroy(GameObject* gm);

	// Select GameObjects
	std::vector<GameObject*>& GetSelectedGOs();

	// If no parameter --> deselect everything
	void SetSelected(GameObject* go = nullptr);
	void SetSelectedState(GameObject* go, bool selected);
	
	// Function to handle GameObject selection by Mouse Picking
	void HandleGameObjectSelection(const LineSegment& ray);

	// Function to test if picking inside an AABB
	bool IsInsideAABB(const float3& point, const AABB& aabb);

	
	void SetCanvas(G_UI* newCanvas = nullptr);
	
	G_UI* ModuleScene::GetCanvas();

	void ReplaceScriptsReferences(uint oldUID, uint newUID);

	void AddToReferenceMap(uint UID, SerializedField* fieldToAdd);

	GameObject* GetGOFromUID(GameObject* n, uint sUID);

	void LoadScriptsData(GameObject* rootObject = nullptr);


public:

	GameObject* mRootNode;

	GameObject* gameCameraObject;
	CCamera* gameCameraComponent;

	std::vector<GameObject*> destroyList;
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> pendingToAdd;

	std::vector<std::string> tags;

	JsonFile ysceneFile;

	Mesh* ourMesh;
	ResourceMesh mymesh{ 0 };

	std::string currentSceneDir;
	std::string currentSceneFile;

	GameObject* selectedGO;
	std::vector<G_UI*> vCanvas;

	std::multimap<uint, SerializedField*> referenceMap;

	//Hardcodeado para la VS1
	GameObject* MainCharacter;

private:

	G_UI* canvas;
	int selectedUI;

	std::vector<GameObject*> vSelectedGOs;

	GameObject* audiosource;

	bool a = false;
};