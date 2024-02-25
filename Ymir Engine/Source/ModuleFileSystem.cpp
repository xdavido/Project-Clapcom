#include "ModuleFileSystem.h"

#include "Log.h"
#include "External/Optick/include/optick.h"

#include "PhysfsEncapsule.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	workingDirectory = "./";
	libraryPath = workingDirectory + "Library/";

	libraryScenesPath = libraryPath + "Scenes/";
	libraryModelsPath = libraryPath + "Models/";
	libraryMeshesPath = libraryPath + "Meshes/";
	libraryMaterialsPath = libraryPath + "Materials/";
	libraryShadersPath = libraryPath + "Shaders/";
	libraryTexturesPath = libraryPath + "Textures/";
	librarySettingsPath = libraryPath + "Settings/";

	LOG("Creating ModuleFileSystem");
}

ModuleFileSystem::~ModuleFileSystem()
{

}

bool ModuleFileSystem::Init()
{
	bool ret = true;

	LOG("Loading File System");

	PhysfsEncapsule::InitializePhysFS();

	if (!PhysfsEncapsule::FolderExists(libraryPath)) {

		CreateLibraryFolder();

	}

	return ret;
}

update_status ModuleFileSystem::PreUpdate(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::Update(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::PostUpdate(float dt)
{
	OPTICK_EVENT();

	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;

	LOG("Deleting File System");

	PhysfsEncapsule::DeinitializePhysFS();

	return ret;
}

void ModuleFileSystem::CreateMetaFileFromAsset(std::string assetPath, JsonFile& jsonFile)
{
	std::string directory;
	std::string fileName;

	if (assetPath.find("/") != std::string::npos) {

		size_t lastSlash = assetPath.find_last_of('/') + 1;

		directory = assetPath.substr(0, lastSlash);
		fileName = assetPath.substr(lastSlash);

	}
	else {

		size_t lastSlash = assetPath.find_last_of('\\') + 1;

		directory = assetPath.substr(0, lastSlash);
		fileName = assetPath.substr(lastSlash);

	}

	jsonFile.CreateJSON(directory, fileName + ".meta");
}

void ModuleFileSystem::CreateLibraryFolder()
{
	PhysfsEncapsule::CreateFolder(workingDirectory, "Library");

	PhysfsEncapsule::CreateFolder(libraryPath, "Scenes"); // Custom File Format (JSON)
	PhysfsEncapsule::CreateFolder(libraryPath, "Models"); // Custom File Format (JSON)
	PhysfsEncapsule::CreateFolder(libraryPath, "Meshes"); // Custom File Format
	PhysfsEncapsule::CreateFolder(libraryPath, "Materials"); // Custom File Format (JSON)
	PhysfsEncapsule::CreateFolder(libraryPath, "Shaders"); // SPIR-V
	PhysfsEncapsule::CreateFolder(libraryPath, "Textures"); // DDS 
	PhysfsEncapsule::CreateFolder(libraryPath, "Settings"); // Custom File Format (JSON)
}

bool ModuleFileSystem::SaveMeshToFile(const Mesh* ourMesh, const std::string& filename) {

	uint bufferSize = 0;
	char* fileBuffer = (char*)ImporterMesh::Save(ourMesh, bufferSize);

	std::ofstream outFile(filename, std::ios::binary);

	if (!outFile.is_open()) {

		LOG("[ERROR] Unable to open the file for writing: %s", filename);

		return false;
	}

	// Write the buffer to the file
	outFile.write(fileBuffer, bufferSize);

	// Close the file
	outFile.close();

	// Free the allocated memory for the buffer
	delete[] fileBuffer;

	return true;
}

bool ModuleFileSystem::SaveTextureToFile(const ResourceTexture* ourTexture, const std::string& filename)
{
	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Set the desired compression format
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

	if (size > 0) {
		data = new ILubyte[size]; // Allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
			std::ofstream file(filename, std::ios::binary); // Open the file
			file.write(reinterpret_cast<const char*>(data), size); // Write data to the file
			file.close(); // Close the file
			delete[] data; // Free the allocated data buffer
			return true; // Return true indicating successful save
		}
	}

	return false; // Return false if saving failed or if size was 0
}

bool ModuleFileSystem::LoadMeshToFile(const std::string filename, ResourceMesh* ourMesh)
{
	// Get size of file to know how much memory to allocate
	std::uintmax_t filesize = std::filesystem::file_size(filename);
	// Allocate buffer to hold file
	char* buf = new char[filesize];
	// Read file
	std::ifstream fin(filename, std::ios::binary);
	fin.read(buf, filesize);
	if (!fin) {

		LOG("[ERROR] File mode read-only, could only read. %f bytes", fin.gcount());
		fin.close();
		return false;
	}
	// Close file
	fin.close();

	ImporterMesh::Load(buf, ourMesh);
	return true;
}
