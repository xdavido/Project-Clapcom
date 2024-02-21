#include "PhysfsEncapsule.h"
#include "Log.h"
#include "Globals.h"

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

void PhysfsEncapsule::DeleteFolder(std::string route)
{
    // Iterate over the directory and delete files
    for (const auto& entry : std::filesystem::directory_iterator(route)) {

        if (std::filesystem::is_directory(entry.path())) {
            // Recursively delete subdirectories
            DeleteFolder(entry.path().string());
        }
        else {
            // Delete files
            if (PHYSFS_delete(entry.path().string().c_str()) == 0) {
            }
        }
    }

    // Delete the empty directory
    if (PHYSFS_delete(route.c_str()) == 0) {



    }

    //// Check if the file exists using std::filesystem
    //if (std::filesystem::exists(route)) {
    //    // Use PhysFS to delete the file
    //    if (PHYSFS_delete(route.c_str()) == 0) {
    //        
    //    }
    //  
    //}
  
}

void PhysfsEncapsule::DeleteFilePhysFS(std::string route)
{
    std::filesystem::remove(route);
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
    //TODO: Compare performance to calling Load(srcFile) and then Save(dstFile)
    std::ifstream src;
    src.open(srcFile, std::ios::binary);
    bool srcOpen = src.is_open();
    std::ofstream  dst(dstFile, std::ios::binary);
    bool dstOpen = dst.is_open();

    dst << src.rdbuf();

    src.close();
    dst.close();

    if (srcOpen && dstOpen)
    {
        LOG("File System: File %s Duplicated Correctly", srcFile);
        return true;
    }
    else
    {
        LOG("[ERROR] File System: Could not be duplicated");
        return false;
    }
    return false;
}
