#include "ModuleResourceManager.h"
#include "ModuleRenderer3D.h"

#include "Log.h"
#include "External/Optick/include/optick.h"
#include "Random.h"
#include "ModuleFileSystem.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "PhysfsEncapsule.h"
#include "JsonFile.h"
#include "GameObject.h"

#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ResourceScene.h"
#include "ResourceTexture.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "ResourceAnimation.h"

#include "ImporterModel.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ImporterMaterial.h"
#include "ImporterShader.h"

#include "External/mmgr/mmgr.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	LOG("Creating ModuleResourceManager");
}

ModuleResourceManager::~ModuleResourceManager()
{

}

bool ModuleResourceManager::Init()
{
	bool ret = true;

	LOG("Loading Resource Manager");

	return ret;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::PostUpdate(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	bool ret = true;

	LOG("Deleting Resource Manager");

	return ret;
}

void ModuleResourceManager::ImportFileToEngine(const char* fileDir)
{
	std::string filePath;
	PhysfsEncapsule::DuplicateFile(fileDir, App->editor->selectedDir.c_str(), filePath);
}

void ModuleResourceManager::ImportFile(const std::string& assetsFilePath, bool onlyReimport)
{
	// Create Meta

	std::string metaFilePath = assetsFilePath + ".meta"; // Assuming the meta file exists.
	std::string path = assetsFilePath;

	// Retrieve info from Meta

	JsonFile* metaFile = JsonFile::GetJSON(metaFilePath);

	if (CheckExtensionType(assetsFilePath.c_str()) == ResourceType::TEXTURE)
	{
		for (auto it = App->scene->GetSelectedGOs().begin(); it != App->scene->GetSelectedGOs().end(); ++it)
		{
			for (auto jt = (*it)->mComponents.begin(); jt != (*it)->mComponents.end(); ++jt)
			{
				if ((*jt)->ctype == ComponentType::MATERIAL)
				{
					ResourceTexture* rTexTemp = new ResourceTexture();
					ImporterTexture::Import(assetsFilePath, rTexTemp);
					
					rTexTemp->type = TextureType::DIFFUSE;
					rTexTemp->UID = Random::Generate();
					static_cast<CMaterial*>((*jt))->path = assetsFilePath;
					static_cast<CMaterial*>((*jt))->rTextures.clear();
					static_cast<CMaterial*>((*jt))->rTextures.push_back(rTexTemp);
				}
				else if (static_cast<C_UI*>((*jt))->UI_type == UI_TYPE::IMAGE)
				{
					ResourceTexture* rTexTemp = new ResourceTexture();
					ImporterTexture::Import(assetsFilePath, rTexTemp);

					rTexTemp->type = TextureType::DIFFUSE;
					rTexTemp->UID = Random::Generate();
					static_cast<UI_Image*>((*jt))->mat->path = assetsFilePath;
					static_cast<UI_Image*>((*jt))->mat->rTextures.clear();
					static_cast<UI_Image*>((*jt))->mat->rTextures.push_back(rTexTemp);
				}

			}

		}

	}
	else
	{
		// If meta file doesn't exist
		if (metaFile == nullptr)
		{
			switch (CheckExtensionType(assetsFilePath.c_str()))
			{
			case ResourceType::UNKNOWN:
				break;
			case ResourceType::TEXTURE:
			{

				/*for (auto itr = 0; itr != App->editor.; itr++)
				{

				}*/
			}
			break;
			case ResourceType::MESH:
			{
				// Rework to ImporterModel::Import(path);
				ReImportModel(path, onlyReimport);
			}
			break;
			case ResourceType::SCENE:
				break;
			case ResourceType::SHADER:
				break;
			case ResourceType::MATERIAL:
				break;
			case ResourceType::META:
				break;
			case ResourceType::ALL_TYPES:
				break;
			case ResourceType::ANIMATION:
				break;
			default:
				break;
			}

		}
		else
		{
			metaFile = JsonFile::GetJSON(metaFilePath);
			uint UID = metaFile->GetInt("UID");

			std::string ext;
			PhysfsEncapsule::SplitFilePath(metaFile->GetString("Library Path").c_str(), nullptr, nullptr, &ext);

			ResourceType type = GetTypeFromString(ext);

			/* The resources that have to be transformed to Ymir Engine format have to be imported,
			but the resources that are already in the custom format only have to be loaded. */

			switch (type) {

			case ResourceType::MESH:

				//ImporterMesh::Load(metaFile->GetString("Library Path").c_str(), (ResourceMesh*)resource);
				break;

			case ResourceType::MODEL:
			{
				//ImporterMesh::Load(metaFile->GetString("Library Path").c_str(), (ResourceMesh*)resource);

				if (!PhysfsEncapsule::FileExists(".\/Library\/Models\/" + std::to_string(metaFile->GetInt("UID")) + ".ymodel")) {

					// Rework to ImporterModel::Import(path);
					ReImportModel(path, onlyReimport);

					break;
				}

				GameObject* modelGO = App->scene->CreateGameObject(metaFile->GetString("Name").c_str(), App->scene->mRootNode);
				modelGO->UID = metaFile->GetInt("UID");
				modelGO->type = "Model";
				modelGO->originPath = assetsFilePath;

				int* ids = metaFile->GetIntArray("Meshes Embedded UID");

				for (int i = 0; i < metaFile->GetInt("Meshes num"); i++)
				{
					// Search resource: if it exists --> create a game object with a reference to it
					// else --> create the resource from library and the game object to contain it
					auto itr = resources.find(ids[i]);

					if (itr == resources.end())
					{
						GameObject* meshGO = App->scene->CreateGameObject(std::to_string(ids[i]), modelGO);
						meshGO->UID = ids[i];
					
						ResourceMesh* rMesh = static_cast<ResourceMesh*>
							(CreateResourceFromLibrary((".\/Library\/Meshes\/" + std::to_string(ids[i]) + ".ymesh").c_str(), ResourceType::MESH, ids[i]));

						CMesh* cmesh = new CMesh(meshGO);

						cmesh->rMeshReference = rMesh;
						cmesh->nVertices = rMesh->vertices.size();
						cmesh->nIndices = rMesh->indices.size();

						meshGO->AddComponent(cmesh);

						// Apply Checker Image

						CMaterial* cmat = new CMaterial(meshGO);

						ResourceTexture* rTex = new ResourceTexture();

						rTex->LoadCheckerImage();

						rTex->type = TextureType::DIFFUSE;

						cmat->UID = rTex->UID;
						cmat->path = "Checker Image";
						cmat->rTextures.push_back(rTex);

						meshGO->AddComponent(cmat);

					}
					else
					{
						GameObject* meshGO = App->scene->CreateGameObject(std::to_string(ids[i]), modelGO);
						meshGO->UID = ids[i];

						ResourceMesh* tmpMesh = static_cast<ResourceMesh*>(itr->second);

						CMesh* cmesh = new CMesh(meshGO);

						cmesh->rMeshReference = tmpMesh;
						cmesh->nVertices = tmpMesh->vertices.size();
						cmesh->nIndices = tmpMesh->indices.size();
			
						meshGO->AddComponent(cmesh);

						// Apply Checker Image

						CMaterial* cmat = new CMaterial(meshGO);
						
						ResourceTexture* rTex = new ResourceTexture();

						rTex->LoadCheckerImage();

						rTex->type = TextureType::DIFFUSE;

						cmat->UID = rTex->UID;
						cmat->path = "Checker Image";
						cmat->rTextures.push_back(rTex);

						meshGO->AddComponent(cmat);

						itr->second->IncreaseReferenceCount();
					}

				}

				//ImporterModel::Import(assetsFilePath.c_str(), (ResourceModel*)resource);

			}
			break;

			case ResourceType::SCENE:

				//ImporterScene::Load(assetsFilePath.c_str(), (ResourceScene*)resource);
				break;

			case ResourceType::TEXTURE:

				//ImporterTexture::Import(assetsFilePath.c_str(), (ResourceTexture*)resource);
				break;

			case ResourceType::MATERIAL:

				//ImporterMaterial::Load(assetsFilePath.c_str(), (ResourceMaterial*)resource);
				break;

			case ResourceType::SHADER:

				//ImporterShader::Import(assetsFilePath.c_str(), (ResourceShader*)resource);
				break;
			case ResourceType::ANIMATION:
				break;
				//ImporterShader::Import(assetsFilePath.c_str(), (ResourceShader*)resource);


			}

		}

	}

}

void ModuleResourceManager::SaveResourceToLibrary(Resource* resource)
{
	switch (resource->GetType()) {

	case ResourceType::MESH:

		//ImporterMesh::Save((ResourceMesh*)resource, resource->GetLibraryFilePath());
		break;

	case ResourceType::MODEL:

		//ImporterModel::Save((ResourceModel*)resource, resource->GetLibraryFilePath());
		break;

	case ResourceType::SCENE:

		//ImporterScene::Save((ResourceScene*)resource, resource->GetLibraryFilePath());
		break;

	case ResourceType::TEXTURE:

		//ImporterTexture::Save((ResourceTexture*)resource, resource->GetLibraryFilePath());
		break;

	case ResourceType::MATERIAL:

		//ImporterMaterial::Save((ResourceMaterial*)resource, resource->GetLibraryFilePath());
		break;

	case ResourceType::SHADER:

		//ImporterShader::Save((ResourceShader*)resource, resource->GetLibraryFilePath());
		break;
	case ResourceType::ANIMATION:
		break;

	}

}

uint ModuleResourceManager::GenerateNewUID()
{
	return Random::Generate();
}

uint ModuleResourceManager::ExistsInLibrary(const std::string& assetsFilePath) const
{
	std::string metaFilePath = assetsFilePath + ".meta";

	if (PhysfsEncapsule::FileExists(metaFilePath))
	{
		JsonFile* metaFile = JsonFile::GetJSON(metaFilePath);

		std::string libraryFilePath = metaFile->GetString("Library Path");

		if (PhysfsEncapsule::FileExists(libraryFilePath)) {

			uint UID = metaFile->GetInt("UID");

			return UID;

		}

	}

	return 0;
}

bool ModuleResourceManager::ExistsInLibrary(ResourceType type, const uint& UID) const
{
	std::string libraryFilePath = resourceTypeToLibraryFolder.at(type) + std::to_string(UID) + "." + resourceTypeToString.at(type);

	if (PhysfsEncapsule::FileExists(libraryFilePath))
	{
		return true;
	}

	return false;
}

void ModuleResourceManager::ReleaseResource(Resource* resource)
{
	resource->UnloadFromMemory();

	resources.erase(resource->GetUID());

	delete resource;
}

void ModuleResourceManager::ReImportModel(const std::string& modelPath, bool onlyReimport)
{
	Model* model = new Model(modelPath, onlyReimport);

	if (onlyReimport) {

		App->scene->SetSelected();
		model->modelGO->mParent->DeleteChild(model->modelGO);

		App->scene->gameObjects.erase(
			std::remove_if(App->scene->gameObjects.begin(), App->scene->gameObjects.end(),
				[](const GameObject* obj) { return obj->selected; }
			),
			App->scene->gameObjects.end()
		);

	}

	delete model;
}

bool ModuleResourceManager::IsResourceLoaded(const uint& UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it != resources.end()) {

		return true;

	}

	return false;
}

void ModuleResourceManager::LoadResource(const uint& UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it == resources.end())
	{
		return;
	}

	Resource* tmpResource = it->second;

	tmpResource->IncreaseReferenceCount();

	tmpResource->LoadInMemory();
}

Resource* ModuleResourceManager::RequestResource(const uint& UID, const char* libraryFilePath)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it != resources.end())
	{
		it->second->IncreaseReferenceCount();

		return it->second;
	}
	else {

		if (libraryFilePath != nullptr)
		{
			Resource* tmpResource = nullptr;

			if (PhysfsEncapsule::FileExists(libraryFilePath))
			{
				ResourceType tmpType = GetTypeFromLibraryPath(libraryFilePath);

				switch (tmpType) {

				case ResourceType::MESH:

					tmpResource = new ResourceMesh(UID);

					break;

				case ResourceType::MODEL:

					tmpResource = new ResourceModel(UID);

					break;

				case ResourceType::SCENE:

					tmpResource = new ResourceScene(UID);

					break;

				case ResourceType::TEXTURE:

					tmpResource = new ResourceTexture(UID);

					break;

				case ResourceType::MATERIAL:

					tmpResource = new ResourceMaterial(UID);

					break;

				case ResourceType::SHADER:

					tmpResource = new ResourceShader(UID);

					break;
				case ResourceType::ANIMATION:
					tmpResource = new ResourceAnimation(UID);

				}

				if (tmpResource != nullptr)
				{
					resources[UID] = tmpResource;

					tmpResource->SetLibraryFilePath(libraryFilePath);

					tmpResource->IncreaseReferenceCount();

					tmpResource->LoadInMemory();
				}

			}

			return tmpResource;

		}
		else {

			LOG("Resource requested isn't loaded and hasn't a library file.");

			return nullptr;

		}

	}

}

bool ModuleResourceManager::UnloadResource(const uint& UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it == resources.end())
	{
		return false;
	}

	Resource* tmpResource = it->second;

	tmpResource->DecreaseReferenceCount();

	if (tmpResource->GetReferenceCount() == 0) {

		ReleaseResource(tmpResource);
		return true;
	}

	return false;
}

Resource* ModuleResourceManager::CreateResourceFromAssets(std::string assetsFilePath, ResourceType type, const uint& UID)
{
	Resource* tmpResource = nullptr;

	switch (type) {

	case ResourceType::MESH:

		tmpResource = new ResourceMesh(UID);
		break;

	case ResourceType::MODEL:

		tmpResource = new ResourceModel(UID);
		break;

	case ResourceType::SCENE:

		tmpResource = new ResourceScene(UID);
		break;

	case ResourceType::TEXTURE:

		tmpResource = new ResourceTexture(UID);
		break;

	case ResourceType::MATERIAL:

		tmpResource = new ResourceMaterial(UID);
		break;

	case ResourceType::SHADER:

		tmpResource = new ResourceShader(UID);
		break;

	case ResourceType::ANIMATION:

		tmpResource = new ResourceAnimation(UID);
		break;

	}

	if (tmpResource != nullptr)
	{
		resources[UID] = tmpResource;

		tmpResource->SetAssetsFilePath(assetsFilePath);

		/*std::string libraryFilePath = GenerateLibraryFile(UID, type);
		tmpResource->SetLibraryFilePath(libraryFilePath);*/

		tmpResource->IncreaseReferenceCount();
	}

	return tmpResource;
}

Resource* ModuleResourceManager::CreateResourceFromLibrary(std::string libraryFilePath, ResourceType type, const uint& UID)
{
	// TODO FRANCESC: Need a smart pointer to solve this memory leak;
	Resource* tmpResource = nullptr;

	switch (type) {

	case ResourceType::MESH:

		tmpResource = new ResourceMesh(UID);
		break;

	case ResourceType::MODEL:

		tmpResource = new ResourceModel(UID);
		break;

	case ResourceType::SCENE:

		tmpResource = new ResourceScene(UID);
		break;

	case ResourceType::TEXTURE:

		tmpResource = new ResourceTexture(UID);
		break;

	case ResourceType::MATERIAL:

		tmpResource = new ResourceMaterial(UID);
		break;

	case ResourceType::SHADER:

		tmpResource = new ResourceShader(UID);
		break;
	case ResourceType::ANIMATION:

		tmpResource = new ResourceAnimation(UID);
		break; 
	}

	if (tmpResource != nullptr)
	{
		// Fix Resource Texture
		
		//if (resources.find(UID) != resources.end()) {

		//	tmpResource->IncreaseReferenceCount();

		//}
		//else {

		//	resources.emplace(UID, tmpResource);

		//	tmpResource->SetLibraryFilePath(libraryFilePath);

		//	tmpResource->LoadInMemory();

		//	tmpResource->IncreaseReferenceCount();

		//}

		// FRANCESC: Disparo arreglado por alg�n motivo si comentas esta l�nea
		//resources.emplace(UID, tmpResource);

		tmpResource->SetLibraryFilePath(libraryFilePath);

		tmpResource->LoadInMemory();

		tmpResource->IncreaseReferenceCount();

	}

	return tmpResource;
}

ResourceType ModuleResourceManager::GetTypeFromAssetsPath(std::string assetsFilePath)
{
	std::string extension = assetsFilePath.substr(assetsFilePath.find_last_of('.') + 1);

	// Convert to lowercase in case the extension is on uppercase
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == "fbx" || extension == "dae" || extension == "obj") return ResourceType::MODEL;
	else if (extension == "yscene") return ResourceType::SCENE;
	else if (extension == "yanim") return ResourceType::ANIMATION;
	else if (extension == "png" || extension == "dds" || extension == "jpg" || extension == "tga") return ResourceType::TEXTURE;

	return ResourceType::UNKNOWN;
}

ResourceType ModuleResourceManager::GetTypeFromLibraryPath(std::string libraryFilePath)
{
	std::string extension = libraryFilePath.substr(libraryFilePath.find_last_of('.') + 1);

	if (extension == "ymesh") return ResourceType::MESH;
	else if (extension == "ymodel") return ResourceType::MODEL;
	else if (extension == "yscene") return ResourceType::SCENE;
	else if (extension == "yanim") return ResourceType::ANIMATION;
	else if (extension == "ymat") return ResourceType::MATERIAL;
	else if (extension == "dds") return ResourceType::TEXTURE;
	else if (extension == "spv") return ResourceType::SHADER;

	return ResourceType::UNKNOWN;
}

ResourceType ModuleResourceManager::GetTypeFromString(std::string typeString)
{
	ResourceType tmpType = ResourceType::UNKNOWN;

	for (const auto& pair : resourceTypeToString) {

		if (pair.second == typeString) {

			tmpType = pair.first;

		}

	}

	return tmpType;
}

std::string ModuleResourceManager::GetStringFromType(ResourceType type)
{
	return resourceTypeToString.at(type);
}

std::map<uint, Resource*> ModuleResourceManager::GetResourcesMap() const
{
	return resources;
}

ResourceType ModuleResourceManager::CheckExtensionType(const char* fileDir)
{
	std::vector<std::string> obj_ext = { "fbx", "FBX", "obj", "OBJ", "DAE", "dae" };
	std::vector<std::string> tex_ext = { "png", "PNG", "jpg", "JPG", "dds", "DDS", "tga", "TGA" };
	std::vector<std::string> meta_ext = { "meta", "json" };

	if (PhysfsEncapsule::HasExtension(fileDir, "yscene"))
	{
		return ResourceType::SCENE;
	}

	if (PhysfsEncapsule::HasExtension(fileDir, meta_ext))
	{
		return ResourceType::META;
	}

	if (PhysfsEncapsule::HasExtension(fileDir, obj_ext))
	{
		return ResourceType::MESH;
	}

	if (PhysfsEncapsule::HasExtension(fileDir, tex_ext))
	{
		return ResourceType::TEXTURE;
	}

	if (PhysfsEncapsule::HasExtension(fileDir, "glsl"))
	{
		return ResourceType::SHADER;
	}

	if (PhysfsEncapsule::HasExtension(fileDir, "yfab"))
	{
		return ResourceType::PREFAB;
	}
	if (PhysfsEncapsule::HasExtension(fileDir, "yanim"))
	{
		return ResourceType::ANIMATION;
	}

	return ResourceType::UNKNOWN;
}
