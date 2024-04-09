#pragma once

#include <map>
#include <string>

#include "Module.h"
#include "Resources.h"
#include "ResourceTexture.h"

class GameObject;

class ModuleResourceManager : public Module
{
public:

	// Constructor & Destructor

	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();

	// Module Functions

	bool Init() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	// Specific Functions
	/// <summary>
	/// If first time import to engine --> Create a copy in local space
	/// If it already exists the file --> Dupe it with another name (nameN.ext) { N == number}
	void ImportFileToEngine(const char* path);
	void ImportFile(const std::string& assetsFilePath, bool onlyReimport = false);
	void SaveResourceToLibrary(Resource* resource);

	uint GenerateNewUID();
	std::string GenerateLibraryFile(const uint& UID, ResourceType type);

	uint ExistsInLibrary(const std::string& assetsFilePath) const;
	bool ExistsInLibrary(ResourceType type, const uint& UID) const;

	bool IsResourceLoaded(const uint& UID);

	Resource* RequestResource(const uint& UID, const char* libraryFilePath = nullptr);
	bool UnloadResource(const uint& UID);

	Resource* CreateResourceFromAssets(std::string assetsFilePath, ResourceType type, const uint& UID);
	Resource* CreateResourceFromLibrary(std::string libraryFilePath, ResourceType type, const uint& UID, TextureType rTexType = TextureType::UNKNOWN);

	ResourceType GetTypeFromAssetsPath(std::string assetsFilePath);
	ResourceType GetTypeFromLibraryPath(std::string libraryFilePath);

	ResourceType GetTypeFromString(std::string typeString);
	std::string GetStringFromType(ResourceType type);

	std::map<uint, Resource*> GetResourcesMap() const;

	ResourceType CheckExtensionType(const char* fileDir);

	void LoadResource(const uint& UID);
	void ReleaseResource(Resource* resource);

	void ReImportModel(const std::string& modelPath, bool onlyReimport);

public:

	std::map<uint, Resource*> resources;

private:

	// Utility maps

	std::map<ResourceType, std::string> resourceTypeToString{

		{ResourceType::MESH, "ymesh"},
		{ResourceType::MODEL, "ymodel"},
		{ResourceType::SCENE, "yscene"},
		{ResourceType::ANIMATION, "yanim"},
		{ResourceType::MATERIAL, "ymat"},
		{ResourceType::TEXTURE, "dds"},
		{ResourceType::SHADER, "spv"},

	};

	std::map<ResourceType, std::string> resourceTypeToLibraryFolder{

		{ResourceType::MESH, "Library/Meshes/"},
		{ResourceType::MODEL, "Library/Models/"},
		{ResourceType::SCENE, "Library/Scenes/"},
		{ResourceType::ANIMATION, "Library/Animations/"},
		{ResourceType::MATERIAL, "Library/Materials/"},
		{ResourceType::TEXTURE, "Library/Textures/"},
		{ResourceType::SHADER, "Library/Shaders/"},

	};

};