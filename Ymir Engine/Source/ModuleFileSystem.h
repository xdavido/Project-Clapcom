#pragma once

#include <string>

#include "Module.h"
#include "ResourceMesh.h"
#include "Texture.h"
#include "ResourceTexture.h"

#include "JsonFile.h"

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, bool start_enabled = true);

	virtual ~ModuleFileSystem();

	bool Init() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void CreateMetaFileFromAsset(std::string assetPath, JsonFile& jsonFile);
	void CreateLibraryFolder();

	// Save
	bool SaveMeshToFile(Mesh* ourMesh, const std::string& filename);
	bool SaveTextureToFile(const ResourceTexture* ourTexture, const std::string& filename);

	// Load
	bool LoadMeshToFile(const std::string filename, ResourceMesh* ourMesh);
	//bool LoadTextureToFile(const Texture* ourTexture, const std::string& filename);

public:

	std::string workingDirectory;
	std::string libraryPath;

	std::string libraryScenesPath;
	std::string libraryModelsPath;
	std::string libraryMeshesPath;
	std::string libraryMaterialsPath;
	std::string libraryShadersPath;
	std::string libraryTexturesPath;
	std::string librarySettingsPath;
	std::string libraryAnimationsPath; 

	JsonFile outputFile;

};