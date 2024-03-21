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

	selectedUI = 0;
	canTab = true;

	return ret;
}

bool ModuleScene::Start()
{
	currentSceneDir = "Assets";

#ifdef _STANDALONE

	LoadSceneFromStart("Assets", "MAPA_FINAL");

#endif // _STANDALONE

	// Test for Physics
	// LoadSceneFromStart("Assets", "PhysicsTest"); 

	// Test for Game Extraction
	// LoadSceneFromStart("Assets", "Water");

	//CreateGUI(UI_TYPE::BUTTON);
	//CreateGUI(UI_TYPE::BUTTON, nullptr, 500, 500);
	//CreateGUI(UI_TYPE::BUTTON, nullptr, 750, 750);

	//CreateGUI(UI_TYPE::SLIDER);

	CreateGUI(UI_TYPE::SLIDER, nullptr, 100, 100);
	//CreateGUI(UI_TYPE::CHECKBOX, nullptr, 500, 500);
	//CreateGUI(UI_TYPE::INPUTBOX, nullptr, 500, 500);
	//CreateGUI(UI_TYPE::TEXT);

	return false;
}

update_status ModuleScene::PreUpdate(float dt)
{
	OPTICK_EVENT();

	/*Destroy gameobjects inside the destroy queue*/
	if (destroyList.size() > 0)
	{
		for (size_t i = 0; i < destroyList.size(); ++i)
		{
			Destroy(destroyList[i]);
		}
		destroyList.clear();
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

		if ((*it)->active) (*it)->Update(dt);

		for (auto jt = (*it)->mComponents.begin(); jt != (*it)->mComponents.end(); ++jt) {

			if ((*jt)->active)(*jt)->Update();

		}

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
			LoadScene(currentSceneDir, currentSceneFile);
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

	gameObjects.insert(gameObjects.end(), pendingToAdd.begin(), pendingToAdd.end());
	pendingToAdd.clear();


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
	//Check if a Game Object with same name exists
	bool exists = false;
	int counter = 0;
	if (gameObjects.size() > 0)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (name == gameObjects[i]->name)    //If the name exists, add 1 to counter
			{
				counter++;
				name = ReName(name, counter);
			}
		}
		return name;
	}
	else return name;
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
	G_UI* tempGameObject = new G_UI(t, pParent == nullptr ? App->scene->mRootNode : pParent, x , y);
	gameObjects.push_back(tempGameObject);

	return tempGameObject;
}

void ModuleScene::ClearScene()
{
	//JsonFile::DeleteJSON(External->fileSystem->libraryScenesPath + std::to_string(mRootNode->UID) + ".yscene");

	uint deletedSceneUID = mRootNode->UID;

	isLocked = false;
	SetSelected();

	// FRANCESC: Doing this RELEASE here makes the meshes disappear
	//RELEASE(mRootNode);

	External->lightManager->lights.clear();
	ClearVec(gameObjects);
	ClearVec(destroyList);
	App->renderer3D->models.clear();

	ClearVec(vTempComponents);
	ClearVec(vCanvas);

	mRootNode = CreateGameObject("Scene", nullptr); // Recreate scene
	mRootNode->UID = deletedSceneUID;
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

	ClearScene();

	gameObjects = sceneToLoad->GetHierarchy("Hierarchy");

	LoadScriptsData();

	RELEASE(sceneToLoad);
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

	delete sceneToLoad;
}

void ModuleScene::Destroy(GameObject* gm)
{
	for (std::vector<GameObject*>::iterator i = gm->mParent->mChildren.begin(); i != gm->mParent->mChildren.end(); ++i)
	{
		if (*i._Ptr == gm)
		{
			gm->mParent->mChildren.erase(i);
			break;
		}
	}
	gm->mParent->mChildren.shrink_to_fit();

	auto it = std::find(gameObjects.begin(), gameObjects.end(), gm);
	if (it != gameObjects.end()) {
		delete* it;
		gameObjects.erase(it);
	}

	gm = nullptr;
}

void ModuleScene::SetActiveState(GameObject* go, bool isActive)
{
	for (auto i = 0; i < go->mChildren.size(); i++)
	{
		if (!go->mChildren.empty())
		{
			SetActiveState(go->mChildren[i], isActive);
		}

		go->mChildren[i]->active = isActive;
	}
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
			if (ray.Intersects(meshToTest->rMeshReference->globalAABB, closest, furthest)) {

				// Test if the mesh is inside another AABB (avoid Skybox selection).
				if (!IsInsideAABB(ray.a, meshToTest->rMeshReference->globalAABB))
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

bool ModuleScene::TabNavigate(bool isForward)
{
	// Get UI elements to navigate
	std::vector<C_UI*> listUI;

	for (int i = 0; i < vCanvas.size(); ++i)
	{
		for (int k = 0; k < vCanvas[i]->mChildren.size(); ++k)
		{
			for (int j = 0; j < vCanvas[i]->mChildren[k]->mComponents.size(); ++j)
			{
				if (static_cast<C_UI*>(vCanvas[i]->mChildren[k]->mComponents[j])->UI_type == UI_TYPE::BUTTON
					|| static_cast<C_UI*>(vCanvas[i]->mChildren[k]->mComponents[j])->UI_type == UI_TYPE::CHECKBOX ||
					static_cast<C_UI*>(vCanvas[i]->mChildren[k]->mComponents[j])->UI_type == UI_TYPE::INPUTBOX ||
					static_cast<C_UI*>(vCanvas[i]->mChildren[k]->mComponents[j])->UI_type == UI_TYPE::SLIDER)
				{
					listUI.push_back(static_cast<C_UI*>(vCanvas[i]->mChildren[k]->mComponents[j]));
				}
			}
		}
	}

	for (auto i = 0; i < listUI.size(); i++)
	{
		if (isForward)
		{
			if (selectedUI == listUI.size() - 1)
			{
				App->scene->SetSelected(listUI[0]->mOwner);

				listUI[selectedUI]->SetState(UI_STATE::NORMAL);
				listUI[0]->SetState(UI_STATE::SELECTED);

				selectedUI = 0;
			}

			else
			{
				App->scene->SetSelected(listUI[selectedUI + 1]->mOwner);

				listUI[selectedUI]->SetState(UI_STATE::NORMAL);
				listUI[selectedUI + 1]->SetState(UI_STATE::SELECTED);

				selectedUI += 1;
			}
		}

		else
		{
			for (auto i = 0; i < listUI.size(); i++)
			{
				if (selectedUI == 0)
				{
					App->scene->SetSelected(listUI[listUI.size() - 1]->mOwner);

					listUI[selectedUI]->SetState(UI_STATE::NORMAL);
					listUI[0]->SetState(UI_STATE::SELECTED);

					selectedUI = listUI.size() - 1;
				}

				else
				{
					App->scene->SetSelected(listUI[selectedUI - 1]->mOwner);

					listUI[selectedUI]->SetState(UI_STATE::NORMAL);
					listUI[selectedUI - 1]->SetState(UI_STATE::SELECTED);

					selectedUI -= 1;
				}
				return true;
			}
		}

		return true;
	}

	return true;
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

