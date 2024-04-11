#include "ModuleScene.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"

#include "GameObject.h"
#include "Log.h"
#include "CCamera.h"
#include "TimeManager.h"

#include "ModuleFileSystem.h"
#include "PhysfsEncapsule.h"
#include "ModuleMonoManager.h"
#include "CScript.h"

#include "External/Optick/include/optick.h"
#include "G_UI.h"

#include "ImporterMesh.h"

#include "External/mmgr/mmgr.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	LOG("Creating ModuleScene");

	mRootNode = CreateGameObject("Scene", nullptr);

	gameCameraObject = CreateGameObject("Main Camera", mRootNode);
	tags = { "Untagged" };

	isLocked = false;

	gameCameraComponent = nullptr;
	canvas = nullptr;
}

ModuleScene::~ModuleScene()
{
	tags.clear();
	delete mRootNode;
}

bool ModuleScene::Init()
{
	bool ret = true;

	LOG("Loading scene");

	CAudioListener* audioListenerComponent = new CAudioListener(gameCameraObject);
	audioListenerComponent->SetAsDefaultListener();
	gameCameraObject->AddComponent(audioListenerComponent);

	CAudioSource* audioSourceComponent = new CAudioSource(gameCameraObject);
	gameCameraObject->AddComponent(audioSourceComponent);

	selectedGO = nullptr;
	godMode = false;

	onHoverUI = 0;
	selectedUIGO = nullptr;
	focusedUIGO = nullptr;
	canTab = true;

	return ret;
}

bool ModuleScene::Start()
{
	currentSceneDir = "Assets";
	//LoadSceneFromStart("Assets/NewFolder", "Player Test"); 
	LoadSceneFromStart("Assets/UI/Inventory", "InventoryScene");

#ifdef _RELEASE

	//LoadSceneFromStart("Assets", "VS2 Release");
	//LoadSceneFromStart("Assets/Scenes", "UI_scene");
	//LoadSceneFromStart("Assets/Scenes", "Start_scene");
	//LoadSceneFromStart("Assets/Scenes", "GameUI");
	//LoadSceneFromStart("Assets", "Enemigo player");
	//LoadSceneFromStart("Assets/UI/Inventory", "InventoryScene");
	/*LoadSceneFromStart("Assets", "Enemigo player"); */
	//LoadSceneFromStart("Assets/Test_Francesc", "TestPrefabs");

#endif // _RELEASE

#ifdef _STANDALONE

	//LoadSceneFromStart("Assets", "VS2 Release");
	//LoadSceneFromStart("Assets/Scenes", "UI_scene");
	//LoadSceneFromStart("Assets/Scenes", "GameUI");
	//LoadSceneFromStart("Assets/Scenes", "Start_scene");
	//LoadSceneFromStart("Assets/Test_Francesc", "TestPrefabs");

#endif // _STANDALONE

	return false;
}

update_status ModuleScene::PreUpdate(float dt)
{
	OPTICK_EVENT();

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		swapList.insert(std::pair<GameObject*, GameObject*>(mRootNode->mChildren[1], mRootNode->mChildren[3]));
		//mRootNode->mChildren[1]->SwapChildren(mRootNode->mChildren[3]);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update(float dt)
{
	OPTICK_EVENT();

	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it)->pendingToDelete) {
			destroyList.push_back((*it));
			continue;
		}

		if ((*it)->active)
		{
			(*it)->Update(dt);

			for (auto jt = (*it)->mComponents.begin(); jt != (*it)->mComponents.end(); ++jt) {

				if ((*jt)->active)(*jt)->Update();

			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	//if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {

	//	ClearScene();

	//}

	if ((App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT) &&
		App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT &&
		TimeManager::gameTimer.GetState() == TimerState::STOPPED) {

		SaveScene(currentSceneDir, currentSceneFile);

	}

	if ((App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT) &&
		App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT &&
		TimeManager::gameTimer.GetState() == TimerState::STOPPED) {

		if (currentSceneFile != "")
		{
			//LoadScene(currentSceneDir, currentSceneFile);
			pendingToAddScene = currentSceneDir + "/" + currentSceneFile + ".yscene";
		}

	}

	// Delete GameObject
	if ((App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN ||
		App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) &&
		!ImGui::GetIO().WantTextInput/* && !App->input->GetInputActive()*/)
	{
		// TODO: Sara --> multiple selection
		//node->mParent->DeleteChild(node);
	}

	// UI navigation
	HandleUINavigation();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	OPTICK_EVENT();

	if (!pendingToAddScene.empty())
	{
		// Obtener el nombre del archivo sin la extensi�n
		std::string name, path;
		PhysfsEncapsule::SplitFilePath(pendingToAddScene.c_str(), &path, &name, nullptr);

		//// Encontrar la posici�n del �ltimo separador de directorio
		//size_t lastSlashPos = pendingToAddScene.find_last_of("/\\");

		//// Si se encontr� el separador de directorio
		//if (lastSlashPos != std::string::npos) {
		//	// Eliminar el nombre del archivo y su extensi�n
		//	pendingToAddScene = pendingToAddScene.substr(0, lastSlashPos);
		//}

		//// Ahora path contiene el directorio sin el nombre del archivo y su extensi�n
		//std::string sceneFileName = name + ".yscene";
		//// Eliminar el nombre del archivo de la ruta completa
		//pendingToAddScene = pendingToAddScene.substr(0, pendingToAddScene.length() - sceneFileName.length());

		LoadScene(path, name);

		pendingToAddScene.clear();
	}

	gameObjects.insert(gameObjects.end(), pendingToAdd.begin(), pendingToAdd.end());
	pendingToAdd.clear();

	/*Destroy gameobjects inside the destroy queue*/
	if (destroyList.size() > 0)
	{
		isLocked = false;
		SetSelected();
		for (size_t i = 0; i < destroyList.size(); ++i)
		{
			Destroy(destroyList[i]);

		}
		destroyList.clear();
	}

	/*swap gameobjects inside the swap queue*/
	if (swapList.size() > 0)
	{
		for (std::map<GameObject*, GameObject*>::iterator it = swapList.begin(); it != swapList.end(); ++it)
		{
			it->first->SwapChildren(it->second);
		}
		swapList.clear();
	}
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;

	LOG("Deleting scene");

	RELEASE(mRootNode);

	return ret;
}

GameObject* ModuleScene::CreateGameObject(std::string name, GameObject* parent)
{
	std::string newName = GetUniqueName(name);

	// TODO FRANCESC: Need a smart pointer to solve this memory leak;
	GameObject* tempGameObject = new GameObject(newName, parent);

	if (parent != nullptr) {

		parent->AddChild(tempGameObject);

	}

	gameObjects.push_back(tempGameObject);

	return tempGameObject;
}

std::string ModuleScene::GetUniqueName(std::string name)
{
	// Check if a Game Object with the same name exists
	bool exists = false;
	int counter = 0;

	// Iterate through existing game object names
	for (auto gameObject : gameObjects) {

		if (name == gameObject->name) {

			exists = true;

			break;

		}

	}

	// If the name already exists, rename it
	if (exists) {

		do {

			counter++;

			name = ReName(name, counter);

			exists = false;

			// Check if the new name already exists

			for (auto gameObject : gameObjects) {

				if (name == gameObject->name) {

					exists = true;

					break;

				}

			}

		} while (exists);

	}

	return name;
}

std::string ModuleScene::ReName(std::string name, uint counter)
{
	std::string uniqueName = name + " (" + std::to_string(counter) + ")";
	std::string newName;

	size_t first = uniqueName.find_first_of("(");

	newName = uniqueName.erase(first - 1);
	newName = uniqueName + " (" + std::to_string(counter) + ")";

	return newName;
}

GameObject* ModuleScene::PostUpdateCreateGameObject(std::string name, GameObject* parent)
{
	GameObject* tempGameObject = new GameObject(name, parent);

	if (parent != nullptr) {

		parent->AddChild(tempGameObject);

	}

	//Creo otro vector de game objects i en el postupdate del scene le meto un push en la lista
	pendingToAdd.push_back(tempGameObject);

	return tempGameObject;
}

void ModuleScene::PostUpdateCreateGameObject_UI(GameObject* go)
{
	pendingToAdd.push_back(go);
}

G_UI* ModuleScene::CreateGUI(UI_TYPE t, GameObject* pParent, int x, int y)
{
	G_UI* tempGameObject = new G_UI(t, pParent == nullptr ? App->scene->mRootNode : pParent, x, y);
	gameObjects.push_back(tempGameObject);

	return tempGameObject;
}

void ModuleScene::ClearScene()
{
	//JsonFile::DeleteJSON(External->fileSystem->libraryScenesPath + std::to_string(mRootNode->UID) + ".yscene");

	isLocked = false;
	SetSelected();

	// FRANCESC: Doing this RELEASE here makes the meshes disappear
	// RELEASE(mRootNode);

	External->resourceManager->resources.clear();

	External->lightManager->lights.clear();

	External->physics->DeleteWorld(); // It was this or nothing :(

	External->physics->CreateWorld();
	ClearVec(gameObjects);
	ClearVec(destroyList);
	App->renderer3D->models.clear();

	ClearVec(vTempComponents);
	ClearVec(vCanvas);
}

void ModuleScene::SaveScene(const std::string& dir, const std::string& fileName)
{
	ysceneFile.SetFloat3("Editor Camera Position", App->camera->editorCamera->GetPos());
	ysceneFile.SetFloat3("Editor Camera Right (X)", App->camera->editorCamera->GetRight());
	ysceneFile.SetFloat3("Editor Camera Up (Y)", App->camera->editorCamera->GetUp());
	ysceneFile.SetFloat3("Editor Camera Front (Z)", App->camera->editorCamera->GetFront());

	ysceneFile.SetHierarchy("Hierarchy", gameObjects);

	if (dir != External->fileSystem->libraryScenesPath)
	{
		App->scene->currentSceneDir = dir;
		App->scene->currentSceneFile = (fileName == "" ? std::to_string(mRootNode->UID) : fileName);

		ysceneFile.CreateJSON(dir + "/", App->scene->currentSceneFile + ".yscene");

		LOG("Scene '%s' saved to %s", App->scene->currentSceneFile.c_str(), App->scene->currentSceneDir.c_str());
	}
	else
	{
		ysceneFile.CreateJSON(dir, std::to_string(mRootNode->UID) + ".yscene");
	}

}

void ModuleScene::LoadScene(const std::string& dir, const std::string& fileName)
{
	if (dir != External->fileSystem->libraryScenesPath)
	{
		App->scene->currentSceneDir = dir;
		App->scene->currentSceneFile = (fileName == "" ? std::to_string(mRootNode->UID) : fileName);

		LOG("Scene '%s' loaded", App->scene->currentSceneFile.c_str(), App->scene->currentSceneDir.c_str());
	}

	JsonFile* sceneToLoad = JsonFile::GetJSON(dir + "/" + (fileName == "" ? std::to_string(mRootNode->UID) : fileName) + ".yscene");

	App->camera->editorCamera->SetPos(sceneToLoad->GetFloat3("Editor Camera Position"));
	App->camera->editorCamera->SetUp(sceneToLoad->GetFloat3("Editor Camera Up (Y)"));
	App->camera->editorCamera->SetFront(sceneToLoad->GetFloat3("Editor Camera Front (Z)"));

	uint deletedSceneUID = mRootNode->UID;

	ClearScene();

	mRootNode = CreateGameObject("Scene", nullptr); // Recreate scene
	mRootNode->UID = deletedSceneUID;

	gameObjects = sceneToLoad->GetHierarchy("Hierarchy");
	mRootNode = gameObjects[0];

	LoadScriptsData();

	RELEASE(sceneToLoad);
}

void ModuleScene::SavePrefab(GameObject* prefab, const std::string& dir, const std::string& fileName)
{
	JsonFile* prefabFile = new JsonFile;

	prefabFile->SetPrefab("Prefab", *prefab);

	prefabFile->CreateJSON(dir + "/", fileName + ".yfab");

	LOG("Prefab '%s' saved to %s", fileName.c_str(), dir.c_str());
}

void ModuleScene::LoadPrefab(const std::string& dir, const std::string& fileName)
{
	ClearVec(vTempComponents);

	JsonFile* prefabToLoad = JsonFile::GetJSON(dir + "/" + fileName + ".yfab");

	// FRANCESC: Bug Hierarchy reimported GO when loading in Case 2
	std::vector<GameObject*> prefab = prefabToLoad->GetHierarchy("Prefab");

	// Add the loaded prefab objects to the existing gameObjects vector
	gameObjects.insert(gameObjects.begin(), prefab.begin(), prefab.end());

	LoadScriptsData();

	LOG("Prefab '%s' loaded", fileName.c_str());

	ClearVec(prefab);
	RELEASE(prefabToLoad);
}

void ModuleScene::LoadSceneFromStart(const std::string& dir, const std::string& fileName)
{
	if (dir != External->fileSystem->libraryScenesPath)
	{
		App->scene->currentSceneDir = dir;
		App->scene->currentSceneFile = (fileName == "" ? std::to_string(mRootNode->UID) : fileName);

		LOG("Scene '%s' loaded", App->scene->currentSceneFile.c_str(), App->scene->currentSceneDir.c_str());
	}

	JsonFile* sceneToLoad = JsonFile::GetJSON(dir + "/" + (fileName == "" ? std::to_string(mRootNode->UID) : fileName) + ".yscene");

	App->camera->editorCamera->SetPos(sceneToLoad->GetFloat3("Editor Camera Position"));
	App->camera->editorCamera->SetUp(sceneToLoad->GetFloat3("Editor Camera Up (Y)"));
	App->camera->editorCamera->SetFront(sceneToLoad->GetFloat3("Editor Camera Front (Z)"));

	ClearScene();

	gameObjects = sceneToLoad->GetHierarchy("Hierarchy");
	mRootNode = gameObjects[0];

	LoadScriptsData();

	RELEASE(sceneToLoad);
}

void ModuleScene::Destroy(GameObject* gm)
{
	for (std::vector<GameObject*>::iterator i = gm->mParent->mChildren.begin(); i != gm->mParent->mChildren.end(); ++i)
	{
		(*i._Ptr)->ClearReferences();

		if (*i._Ptr == gm)
		{
			gm->mParent->mChildren.erase(i);
			break;
		}
	}

	gm->mParent->mChildren.shrink_to_fit();

	RELEASE(gm);
}

//
std::vector<GameObject*>& ModuleScene::GetSelectedGOs()
{
	return vSelectedGOs;
}

void ModuleScene::SetSelected(GameObject* go)
{
	if (!isLocked)
	{
		if (go != nullptr)
		{
			// If ctrl not pressed, set everything to false clear and the selected go's vector 
			if (!ImGui::GetIO().KeyCtrl)
			{
				for (auto i = 0; i < vSelectedGOs.size(); i++)
				{
					SetSelectedState(vSelectedGOs[i], false);
				}
				ClearVec(vSelectedGOs);
			}

			// On click select or deselect item
			go->selected = !go->selected;

			// If the item was selected, add it to the vec, otherwise remove it
			if (go->selected)
			{
				selectedGO = go;

				vSelectedGOs.push_back(go);

				// Set selected go children to the same state as the clicked item
				SetSelectedState(go, go->selected);
			}
			else if (!vSelectedGOs.empty())
			{
				SetSelectedState(go, false);
				vSelectedGOs.erase(std::find(vSelectedGOs.begin(), vSelectedGOs.end(), go));
			}
		}
		else
		{
			selectedGO = nullptr;

			for (auto i = 0; i < vSelectedGOs.size(); i++)
			{
				SetSelectedState(vSelectedGOs[i], false);
			}

			ClearVec(vSelectedGOs);
		}
	}
}

void ModuleScene::SetSelectedState(GameObject* go, bool selected)
{
	// Must change go value manually. In "active" not necessary since it changes from the toggle

	if (go != nullptr)
	{
		go->selected = selected;

		for (auto i = 0; i < go->mChildren.size(); i++)
		{
			if (!go->mChildren.empty())
			{
				SetSelectedState(go->mChildren[i], selected);
			}

			go->mChildren[i]->selected = selected;

			if (go->mChildren[i]->selected)
			{
				vSelectedGOs.push_back(go->mChildren[i]);
			}
		}
	}
}

void ModuleScene::SetActiveRecursively(GameObject* gameObject, bool active)
{
	gameObject->active = active;

	for (auto& child : gameObject->mChildren) {

		SetActiveRecursively(child, active);

	}
}

// Function to handle GameObject selection by Mouse Picking
void ModuleScene::HandleGameObjectSelection(const LineSegment& ray)
{
	// Map to store mesh candidates based on their distance to the ray origin.
	std::map<float, CMesh*> meshCandidates;

	// Iterate through all models in the 3D renderer.
	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		CMesh* meshToTest = (CMesh*)(*it)->GetComponent(ComponentType::MESH);

		// Variables to store the closest and furthest intersection distances.
		float closest;
		float furthest;

		if (meshToTest != nullptr)
		{
			// Check for intersection between the ray and the global axis-aligned bounding box (AABB) of the mesh.
			if (ray.Intersects(meshToTest->globalAABB, closest, furthest)) {

				// Test if the mesh is inside another AABB (avoid Skybox selection).
				if (!IsInsideAABB(ray.a, meshToTest->globalAABB))
				{
					// Store the mesh in the map based on the closest intersection distance.
					meshCandidates[closest] = meshToTest;
				}

			}
		}
	}

	// Vector to store meshes sorted by their distance to the ray origin.
	std::vector<CMesh*> meshesSorted;

	// Populate the sorted vector based on the map.
	for (auto& candidate : meshCandidates) {

		meshesSorted.push_back(candidate.second);

	}

	// Set all meshes to unselected initially.
	/*for (CMesh* mesh : meshesSorted) {

		if (mesh != nullptr && mesh->mOwner != nullptr) {

			SetSelected();

		}

	}*/

	// Iterate through the sorted meshes to find the first intersection with the ray.
	for (CMesh* mesh : meshesSorted) {

		if (mesh != nullptr) {

			// Create a local copy of the ray to transform based on the mesh's transform.
			LineSegment localRay = ray;

			// Transform the ray using the mesh's transform.
			localRay.Transform(mesh->mOwner->mTransform->mGlobalMatrix.Inverted());

			// Iterate over triangles in the mesh.
			for (uint j = 0; j < mesh->rMeshReference->indices.size(); j += 3) {

				uint triangle_indices[3] = { mesh->rMeshReference->indices[j], mesh->rMeshReference->indices[j + 1], mesh->rMeshReference->indices[j + 2] };

				// Access mesh vertices.
				float3 point_a(mesh->rMeshReference->vertices[triangle_indices[0]].position);
				float3 point_b(mesh->rMeshReference->vertices[triangle_indices[1]].position);
				float3 point_c(mesh->rMeshReference->vertices[triangle_indices[2]].position);

				// Create a triangle from the vertices.
				Triangle triangle(point_a, point_b, point_c);

				// Check for intersection between the transformed ray and the triangle.
				if (localRay.Intersects(triangle, nullptr, nullptr)) {

					// Intersection found, set the selected object.
					if (mesh->mOwner != nullptr) {

						App->scene->SetSelected(mesh->mOwner);

						//// Iterate through all game objects in the scene.
						//for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it) {

						//	// Unselect other game objects.
						//	if ((*it) != mesh->mOwner) {

						//		(*it)->selected = false;

						//	}

						//}

					}

					// Exit the function after the first intersection is found.
					return;

				}

			}

		}

	}

	SetSelected();

	//// No intersection found, clear the selection for all meshes.
	//for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it) {

	//	(*it)->selected = false;

	//}

}

bool ModuleScene::IsInsideAABB(const float3& point, const AABB& aabb)
{
	return point.x >= aabb.minPoint.x
		&& point.x <= aabb.maxPoint.x
		&& point.y >= aabb.minPoint.y
		&& point.y <= aabb.maxPoint.y
		&& point.z >= aabb.minPoint.z
		&& point.z <= aabb.maxPoint.z;
}

// GUI
void ModuleScene::SetCanvas(G_UI* newCanvas)
{
	canvas = newCanvas;
}

G_UI* ModuleScene::GetCanvas()
{
	return canvas;
}

//
GameObject* ModuleScene::GetGOFromUID(GameObject* n, uint sUID)
{
	if (n->UID == sUID)
		return n;

	GameObject* ret = nullptr;
	for (size_t i = 0; i < n->mChildren.size(); i++)
	{
		ret = GetGOFromUID(n->mChildren[i], sUID);
		if (ret != nullptr)
			return ret;
	}

	return nullptr;
}

void ModuleScene::ReplaceScriptsReferences(uint oldUID, uint newUID)
{
	std::multimap<uint, SerializedField*>::iterator referenceIt = referenceMap.find(oldUID);

	if (referenceIt != referenceMap.end())
	{
		AddToReferenceMap(newUID, referenceIt->second);
		referenceMap.erase(oldUID);
	}
}

void ModuleScene::AddToReferenceMap(uint UID, SerializedField* fieldToAdd)
{
	referenceMap.emplace(UID, fieldToAdd);
}

void ModuleScene::LoadScriptsData(GameObject* rootObject)
{
	std::multimap<uint, SerializedField*> referenceMapCopy;
	for (auto i = referenceMap.begin(); i != referenceMap.end(); ++i)
	{
		// Get the range of the current key
		auto range = referenceMap.equal_range(i->first);

		// Now render out that whole range
		for (auto d = range.first; d != range.second; ++d)
		{
			//if (d->second->fiValue.goValue != nullptr)
				//continue;

			if (rootObject != nullptr)
			{
				GameObject* gameObject = GetGOFromUID(rootObject, d->first);

				if (gameObject != nullptr)
					d->second->fiValue.goValue = gameObject;
				else
					d->second->fiValue.goValue = GetGOFromUID(External->scene->mRootNode, d->first);
			}
			else
			{
				d->second->fiValue.goValue = GetGOFromUID(External->scene->mRootNode, d->first);
			}

			if (d->second->fiValue.goValue != nullptr)
			{
				//d->second->goUID = d->first;

				if (std::find(d->second->fiValue.goValue->csReferences.begin(), d->second->fiValue.goValue->csReferences.end(), d->second) == d->second->fiValue.goValue->csReferences.end())
					d->second->fiValue.goValue->csReferences.push_back(d->second);

				d->second->parentSC->SetField(d->second->field, d->second->fiValue.goValue);

				//d->second = nullptr;
			}
		}
	}

	//for (auto i = referenceMap.begin(); i != referenceMap.end(); ++i)
	//{
	//	if (i->second != nullptr)
	//	{
	//		referenceMapCopy.emplace(i->first, i->second);
	//	}
	//}

	//referenceMap = referenceMapCopy;

	referenceMap.clear();
}

void ModuleScene::GetUINavigate(GameObject* go, std::vector<C_UI*>& listgo)
{
	if (go->active)
	{
		for (auto i = 0; i < static_cast<G_UI*>(go)->mComponents.size(); i++)
		{
			if (static_cast<G_UI*>(go)->mComponents[i]->ctype == ComponentType::UI && static_cast<C_UI*>(static_cast<G_UI*>(go)->mComponents[i])->tabNav_ &&
				static_cast<C_UI*>(static_cast<G_UI*>(go)->mComponents[i])->state != UI_STATE::DISABLED)
			{
				listgo.push_back((C_UI*)static_cast<G_UI*>(go)->mComponents[i]);
			}
		}
	}

	if (!go->mChildren.empty())
	{
		for (auto i = 0; i < go->mChildren.size(); i++)
		{
			GetUINavigate(go->mChildren[i], listgo);
		}
	}
}

GameObject* ModuleScene::GetUISelected(GameObject* go)
{
	if (go->active)
	{
		if (!go->mChildren.empty())
		{
			for (auto i = 0; i < go->mChildren.size(); i++)
			{
				for (auto i = 0; i < static_cast<G_UI*>(go->mChildren[i])->mComponents.size(); i++)
				{
					if (static_cast<C_UI*>(static_cast<G_UI*>(go->mChildren[i])->mComponents[i])->state == UI_STATE::SELECTED)
					{
						return go;
					}
				}
			}
		}
	}

	return nullptr;
}

void ModuleScene::ResetSelected()
{
	// Get UI elements to navigate
	std::vector<C_UI*> listUI;

	for (int i = 0; i < vCanvas.size(); ++i)
	{
		GetUINavigate(vCanvas[i], listUI);
	}

	for (auto i = 0; i < listUI.size(); i++)
	{
		if (listUI[i]->state == UI_STATE::SELECTED)
		{
			listUI[i]->state = UI_STATE::NORMAL;
		}

	}
}


void ModuleScene::TabNavigate(bool isForward)
{
	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		// Get UI elements to navigate
		std::vector<C_UI*> listUI;

		for (int i = 0; i < vCanvas.size(); ++i)
		{
			GetUINavigate(vCanvas[i], listUI);
		}

		if (!listUI.empty())
		{
			if (isForward)
			{
				if (onHoverUI == listUI.size() - 1)
				{
					SetSelected(listUI[0]->mOwner);

					focusedUIGO = listUI[0]->mOwner;

					if (listUI[onHoverUI]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI]->SetState(UI_STATE::NORMAL);
					}

					if (listUI[0]->state != UI_STATE::SELECTED)
					{
						listUI[0]->SetState(UI_STATE::FOCUSED);
					}

					onHoverUI = 0;
				}

				else
				{
					SetSelected(listUI[onHoverUI + 1]->mOwner);

					focusedUIGO = listUI[onHoverUI + 1]->mOwner;

					if (listUI[onHoverUI]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI]->SetState(UI_STATE::NORMAL);
					}

					if (listUI[onHoverUI + 1]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI + 1]->SetState(UI_STATE::FOCUSED);
					}

					onHoverUI += 1;
				}
			}

			else
			{

				if (onHoverUI == 0)
				{
					SetSelected(listUI[listUI.size() - 1]->mOwner);
					focusedUIGO = listUI[listUI.size() - 1]->mOwner;

					if (listUI[onHoverUI]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI]->SetState(UI_STATE::NORMAL);
					}

					if (listUI[listUI.size() - 1]->state != UI_STATE::SELECTED)
					{
						listUI[listUI.size() - 1]->SetState(UI_STATE::FOCUSED);
					}

					onHoverUI = listUI.size() - 1;
				}

				else
				{
					SetSelected(listUI[onHoverUI - 1]->mOwner);
					focusedUIGO = listUI[onHoverUI - 1]->mOwner;

					if (listUI[onHoverUI]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI]->SetState(UI_STATE::NORMAL);
					}

					if (listUI[onHoverUI - 1]->state != UI_STATE::SELECTED)
					{
						listUI[onHoverUI - 1]->SetState(UI_STATE::FOCUSED);
					}

					onHoverUI -= 1;
				}
			}
		}
	}
}

void ModuleScene::HandleUINavigation()
{
	if (!canTab && App->input->GetGamepadLeftJoystickPositionValueY() == 0)
	{
		canTab = true;
	}

	if ((App->input->GetGamepadLeftJoystickPositionValueY() < 0 && canTab) || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		canTab = false;
		TabNavigate(false);
	}

	else if ((App->input->GetGamepadLeftJoystickPositionValueY() > 0 && canTab) || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		canTab = false;
		TabNavigate(true);
	}
}

