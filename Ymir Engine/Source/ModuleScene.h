#pragma once

#include <vector>
#include <map>

#include "Module.h"
#include "ModuleFileSystem.h"
#include "ModuleMonoManager.h"
#include "ModuleLightManager.h"

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
	GameObject* PostUpdateCreateGameObject(std::string name, GameObject* parent);

	void PostUpdateCreateGameObject_UI(GameObject* go);
	std::string GetUniqueName(std::string name);
	std::string ReName(std::string name, uint counter);

	G_UI* CreateGUI(UI_TYPE t, GameObject* pParent = nullptr, int x = 0, int y = 0);
	//void DestroyGameObject(GameObject* toDestroy);

	void ClearScene();

	void SaveScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");
	void LoadScene(const std::string& dir = External->fileSystem->libraryScenesPath, const std::string& fileName = "");

	void SavePrefab(GameObject* prefab, const std::string& dir, const std::string& fileName);
	void LoadPrefab(const std::string& dir, const std::string& fileName);

	// Start with a loaded scene from start
	void LoadSceneFromStart(const std::string& dir, const std::string& fileName);
	
	void Destroy(GameObject* gm);

	// Select GameObjects
	std::vector<GameObject*>& GetSelectedGOs();

	// If no parameter --> deselect everything
	void SetSelected(GameObject* go = nullptr);
	void SetSelectedState(GameObject* go, bool selected);
	void SetActiveRecursively(GameObject* gameObject, bool active = true);
	
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

	// UI navigation
	void GetUINavigate(GameObject* go, std::vector<C_UI*>& listgo);
	GameObject* GetUISelected(GameObject* go);
	void ResetSelected();
	void TabNavigate(bool isForward);

	// Handle both keyboard and gamepad control of all UI game objects
	void HandleUINavigation();

public:

	GameObject* mRootNode;

	GameObject* gameCameraObject;
	CCamera* gameCameraComponent;

	std::vector<GameObject*> destroyList;
	std::map<GameObject*, GameObject*> swapList;
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> pendingToAdd;

	std::vector<std::string> tags;

	JsonFile ysceneFile;

	Mesh* ourMesh;
	ResourceMesh mymesh{ 0 };

	std::string currentSceneDir;
	std::string currentSceneFile;
	std::string pendingToAddScene;

	bool isLocked;
	GameObject* selectedGO;
	std::vector<G_UI*> vCanvas;
	int onHoverUI;

	std::multimap<uint, SerializedField*> referenceMap;

	// God mode
	bool godMode;
	GameObject* selectedUIGO;
	GameObject* focusedUIGO;

private:
	G_UI* canvas;

	std::vector<GameObject*> vSelectedGOs;

	// Limit tab navigation with controller
	bool canTab;
	
	GameObject* audiosource;	

public:
	// DO NOT USE, Save/Load purposes only
    std::vector<Component*> vTempComponents;

};