#include "PhysfsEncapsule.h"
#include "Log.h"
#include "Globals.h"

#include "External/mmgr/mmgr.h"


void PhysfsEncapsule::InitializePhysFS()
{
	if (PHYSFS_init(NULL) == 0) {
		// Handle initialization error
	}
}

bool PhysfsEncapsule::FolderExists(std::string route)
{
	return std::filesystem::exists(route) && std::filesystem::is_directory(route);
}

bool PhysfsEncapsule::FileExists(std::string route)
{
	return std::filesystem::exists(route) && std::filesystem::is_regular_file(route);
}

void PhysfsEncapsule::CreateFolder(std::string route, std::string folderName)
{
	// Set the write directory (this is where you can create folders and files)
	if (PHYSFS_setWriteDir(route.c_str()) == 0) {
		// Handle setting write directory error

	}

	// Create a folder (directory)
	if (PHYSFS_mkdir(folderName.c_str()) == 0) {
		// Handle folder creation error
		// Note: If the folder already exists, PHYSFS_mkdir will return success.
		// You may want to check if the folder already exists before attempting to create it.

	}
}

void PhysfsEncapsule::ReadFolder(std::string route)
{
	/* PHYSFS_Stat stat;
	   char** files = PHYSFS_enumerateFiles(route.c_str());
	   char** i;

	   for (i = files; *i != NULL; i++) {
		   if (PHYSFS_stat(*i, &stat) != 0) {
			   if (stat.filetype == PHYSFS_FILETYPE_REGULAR) {
				   LOG("File: %s", *i);
			   }
			   else if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
				   LOG("Directory: %s", *i);
			   }
		   }
	   }

	   PHYSFS_freeList(files);*/
}

void PhysfsEncapsule::DeleteFS(std::string route)
{
	// It calls std::filesystem::remove(route); recursively in case its a folder
	std::filesystem::remove_all(route);
}

bool PhysfsEncapsule::CopyFileFromSource(std::string source, std::string destination)
{
	PHYSFS_File* srcFile = PHYSFS_openRead(source.c_str());
	if (srcFile == nullptr) {

		return false;
	}

	PHYSFS_File* destFile = PHYSFS_openWrite(destination.c_str());
	if (destFile == nullptr) {

		PHYSFS_close(srcFile);
		return false;
	}

	const int bufferSize = 4096;
	char buffer[bufferSize];

	PHYSFS_sint64 bytesRead;
	while ((bytesRead = PHYSFS_read(srcFile, buffer, 1, bufferSize)) > 0) {
		PHYSFS_write(destFile, buffer, 1, static_cast<PHYSFS_uint32>(bytesRead));
	}

	PHYSFS_close(srcFile);
	PHYSFS_close(destFile);

	return true;
}

void PhysfsEncapsule::DeinitializePhysFS()
{
	// Deinitialize PhysFS
	PHYSFS_deinit();
}

uint PhysfsEncapsule::SaveFile(const char* file, const void* buffer, uint size, bool append)
{
	uint ret = 0;

	bool overwrite;

	(PHYSFS_exists(file) == 0) ? overwrite = false : overwrite = true;

	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);

		if (written != size)
		{
			LOG("[ERROR] [File System] Could not write to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("[File System] Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("[File System] File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
			}
			else
			{
				LOG("[File System] New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
			}

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
		{
			LOG("[ERROR] [File System] Could not close file %s: %s", file, PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("[ERROR] [File System] Could not open file %s: %s", file, PHYSFS_getLastError());
	}

	return ret;
}

uint PhysfsEncapsule::LoadFileToBuffer(const char* file, char** buffer)
{
	uint ret = 0;
	std::filesystem::path filepath(file);

	if (std::filesystem::exists(filepath) && !std::filesystem::is_directory(filepath))
	{
		std::ifstream fs_file(filepath, std::ios::binary | std::ios::ate);

		if (fs_file.is_open())
		{
			std::streamsize size = fs_file.tellg();
			fs_file.seekg(0, std::ios::beg);

			if (size > 0)
			{
				*buffer = new char[size + 1];
				if (fs_file.read(*buffer, size))
				{
					ret = static_cast<uint>(size);
					(*buffer)[size] = '\0';
				}
				else
				{
					LOG("[ERROR] File System: Could not read from file %s\n", file);
					delete[] * buffer;
					*buffer = nullptr;
				}
			}
			fs_file.close();
		}
		else
		{
			LOG("[ERROR] File System: Could not open file %s\n", file);
		}
	}
	else
	{
		LOG("[ERROR] File System: File does not exist or is a directory: %s\n", file);
	}

	return ret;
}

bool PhysfsEncapsule::IsDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

void PhysfsEncapsule::DiscoverFiles(const char* directory, std::vector<std::string>& vFiles, std::vector<std::string>& vDirs)
{
	char** rc = PHYSFS_enumerateFiles(directory);

	for (char** i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);

		(IsDirectory(str.c_str())) ? vDirs.push_back(*i) : vFiles.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

std::string PhysfsEncapsule::GetUniqueName(const char* path, const char* name)
{
	//TODO: modify to distinguix files and dirs?
	std::vector<std::string> vFiles, vDirectories;
	DiscoverFiles(path, vFiles, vDirectories);

	std::string finalName(name);
	bool unique = false;

	for (uint i = 0; i < 50 && unique == false; ++i)
	{
		unique = true;

		// Build the compare name (name_i)
		if (i > 0)
		{
			finalName = std::string(name).append("_");

			if (i < 10)
			{
				finalName.append("0");
			}
			finalName.append(std::to_string(i));
		}

		// Iterate through all the files to find a matching name
		for (uint f = 0; f < vFiles.size(); ++f)
		{
			std::string filePath, fileName, fileExt;
			SplitFilePath(vFiles[f].c_str(), &filePath, &fileName, &fileExt);

			if (finalName == fileName)
			{
				unique = false;
				break;
			}
		}
	}

	return finalName;
}

void PhysfsEncapsule::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension)
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
			{
				*path = full.substr(0, pos_separator + 1);
			}
			else
			{
				path->clear();
			}
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
			{
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			}
			else
			{
				*file = full.substr(0, pos_dot);
			}
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
			{
				*extension = full.substr(pos_dot + 1);
			}
			else
			{
				extension->clear();
			}
		}
	}
}

bool PhysfsEncapsule::HasExtension(const char* path)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool PhysfsEncapsule::HasExtension(const char* path, std::string extension)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);

	return ext == extension;
}

bool PhysfsEncapsule::HasExtension(const char* path, std::vector<std::string> extensions)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
	{
		return true;
	}

	for (uint i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
		{
			return true;
		}
	}

	return false;
}

bool PhysfsEncapsule::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	std::string finalName = dstFolder;
	std::string name = finalName + "/" + GetUniqueName(dstFolder, fileStr.c_str()) + "." + extensionStr;
	return DuplicateFile(file, name.c_str());
}

bool PhysfsEncapsule::DuplicateFile(const char* srcFile, const char* dstFile)
{
	try {

		std::filesystem::copy_file(srcFile, dstFile, std::filesystem::copy_options::overwrite_existing);

		LOG("File System: File %s duplicated correctly.\n", srcFile);

		return true;

	}
	catch (const std::filesystem::filesystem_error& e) {

		LOG("[ERROR] File System: %s\n", e.what());

		return false;

	}

}

bool PhysfsEncapsule::RenameFile(std::string oldFile, std::string newFile) {

	try {
		// Check if the old file exists
		if (!std::filesystem::exists(oldFile)) {
			// Old file does not exist
			return false;
		}

		// Rename the file
		std::filesystem::rename(oldFile, newFile);

		return true;
	}
	catch (const std::filesystem::filesystem_error& e) {
		// An error occurred while renaming the file
		// You can handle the error here
		return false;
	}

}

std::string PhysfsEncapsule::ConvertFileName(const std::string& name)
{
	// Find the position of the last dot
		size_t dotPosition = name.find_last_of('.');
	std::string nameWithoutExtension = name.substr(0, dotPosition); // Get part of the name before the dot

	std::string convertedName = nameWithoutExtension;

	// Replace spaces with underscores
	for (char& c : convertedName) {
		if (c == ' ') {
			c = '_';
		}
	}
	return convertedName;
}



std::string PhysfsEncapsule::UnNormalizePath(const char* full_path)
{
    std::string newPath(full_path);
    for (int i = 0; i < newPath.size(); ++i)
    {
        if (newPath[i] == '/')
            newPath[i] = '\\';
    }
    return newPath;
}

std::string PhysfsEncapsule::NormalizePath(const char* full_path)
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

// Read a whole file and put it in a new buffer
uint PhysfsEncapsule::LoadToBuffer(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);
		//LOG(LogType::L_ERROR, "[%s]", PHYSFS_getLastError())

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG("[ERROR] File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[ERROR] File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("[ERROR] File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

std::string PhysfsEncapsule::GetAssetName(const std::string& path) {
	// Find the last occurrence of '/' or '\\' in the path
	size_t lastSlashPos = path.find_last_of("/\\");

	// If no slash is found, return the entire path
	if (lastSlashPos == std::string::npos)
		return path;

	// Get the substring after the last slash
	std::string name = path.substr(lastSlashPos + 1);

	// Find the dot (.) to get the name without extension
	size_t dotPos = name.find_last_of(".");

	// If no dot is found, return the full name
	if (dotPos == std::string::npos)
		return name;

	// Return only the name without extension
	return name.substr(0, dotPos);
}

// Función genérica para procesar un archivo y combinar los valores según los campos especificados
std::string PhysfsEncapsule::ExtractStringFromSVG(const std::string& filename, const std::vector<std::string>& fieldNames) {
	std::ifstream file(filename);
	std::string line;
	std::vector<std::unordered_map<std::string, std::string>> data; // Vector de mapas (campo -> valor)

	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo " << filename << std::endl;
		LOG("[ERROR] No se pudo abrir el archivo %s", filename);
		return "";
	}

	// Leer cada línea del archivo
	while (std::getline(file, line)) {
		std::unordered_map<std::string, std::string> record; // Mapa para almacenar los valores de cada línea

		// Procesar la línea para cada nombre de campo especificado
		for (const auto& fieldName : fieldNames) {
			size_t pos = line.find(fieldName);
			if (pos != std::string::npos) {
				// Encontrar el valor después del nombre del campo
				std::string value = line.substr(pos + fieldName.length());
				// Eliminar espacios en blanco al principio y al final del valor
				value.erase(0, value.find_first_not_of(" \t\r\n"));
				value.erase(value.find_last_not_of(" \t\r\n") + 1);

				// Almacenar el par (campo, valor) en el mapa
				record[fieldName] = value;
			}
		}

		if (!record.empty()) {
			// Agregar el mapa de esta línea al vector de datos
			data.push_back(record);
		}
	}

	file.close();

	// Construir la cadena de salida en el formato deseado
	std::stringstream result;
	for (const auto& record : data) {
		bool firstField = true;
		for (const auto& fieldName : fieldNames) {
			if (record.find(fieldName) != record.end()) {
				if (!firstField) {
					result << ",";
				}
				result << record.at(fieldName);
				firstField = false;
			}
		}
		result << ";";
	}

	return result.str();
}