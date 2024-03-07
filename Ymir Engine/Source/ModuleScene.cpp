#include "ModuleScene.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"

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
	// UID regenerated = duplication (This will be fixed with scene serialization)

	mRootNode = CreateGameObject("Scene", nullptr);
	mRootNode->UID = Random::Generate();

	//Hardcodeado para la VS1
	MainCharacter = CreateGameObject("Main Character", mRootNode);

	gameCameraObject = CreateGameObject("Main Camera", MainCharacter);
	gameCameraObject->UID = Random::Generate();
	tags = { "Untagged" };

	//audiosource = CreateGameObject("AudioSource", mRootNode);
	//audiosource->UID = Random::Generate();

	gameCameraComponent = nullptr;
	canvas = nullptr;

	LOG("Creating ModuleScene");
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

	//Hardcodeado para la VS1
	CScript* scriptComponent = new CScript(MainCharacter,"PlayerMovement");
	MainCharacter->AddComponent(scriptComponent);

	//CAudioSource* audioSourceComponent = new CAudioSource(gameCameraObject);
	//gameCameraObject->AddComponent(audioSourceComponent);
	
	selectedGO = nullptr;

	return ret;
}

bool ModuleScene::Start()
{
	currentSceneDir = "Assets";

	// Test for Physics
	// LoadSceneFromStart("Assets", "PhysicsTest"); 

	// Test for Game Extraction
	// LoadSceneFromStart("Assets", "Water");
	
	//Audio Testing
#ifdef _STANDALONE
	LoadSceneFromStart("Assets", "BakerHouse");
#endif // _STANDALONE

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
		
		if ((*it)->pendingToDelet) {
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
	// TODO FRANCESC: Need a smart pointer to solve this memory leak;
	GameObject* tempGameObject = new GameObject(name, parent);

	if (parent != nullptr) {

		parent->AddChild(tempGameObject);

	}

	
		gameObjects.push_back(tempGameObject);
	

	return tempGameObject;
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

G_UI* ModuleScene::CreateGUI(UI_TYPE t, GameObject* pParent, int x, int y)
{
	G_UI* tempGameObject = new G_UI(t, pParent == nullptr ? App->scene->mRootNode : pParent);
	gameObjects.push_back(tempGameObject);

	return tempGameObject;
}



//void ModuleScene::DestroyGameObject(GameObject* toDestroy)
//{
//	if (toDestroy) {

//		toDestroy->DestroyGameObject();

//	}

//}

void ModuleScene::ClearScene()
{
	//JsonFile::DeleteJSON(External->fileSystem->libraryScenesPath + std::to_string(mRootNode->UID) + ".yscene");

	uint deletedSceneUID = mRootNode->UID;

	/*App->editor->DestroyHierarchyTree(mRootNode);
	delete mRootNode;
	mRootNode = nullptr;*/

	SetSelected();

	RELEASE(mRootNode);

	gameObjects.clear();
	destroyList.clear();
	App->renderer3D->models.clear();
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

		ysceneFile.CreateJSON(dir +"/", App->scene->currentSceneFile + ".yscene");

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
	mRootNode = gameObjects[0];
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

	// ClearScene();

	gameObjects = sceneToLoad->GetHierarchy("Hierarchy");
	mRootNode = gameObjects[0];

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

std::vector<GameObject*>& ModuleScene::GetSelectedGOs()
{
	return vSelectedGOs;
}

void ModuleScene::SetSelected(GameObject* go)
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

void ModuleScene::SetCanvas(G_UI* newCanvas)
{
	canvas = newCanvas;
}

G_UI* ModuleScene::GetCanvas()
{
	return canvas;
}

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