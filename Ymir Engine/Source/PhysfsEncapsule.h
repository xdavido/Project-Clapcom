#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <filesystem>

#include "External/PhysFS/include/physfs.h"
#pragma comment (lib, "Source/External/PhysFS/libx86/physfs.lib")

namespace PhysfsEncapsule {

    void InitializePhysFS();

    bool FolderExists(std::string route);
    bool FileExists(std::string route);
    void CreateFolder(std::string route, std::string folderName);
    void ReadFolder(std::string route);
    void DeleteFolder(std::string route);
    void DeleteFilePhysFS(std::string route);

    bool CopyFileFromSource(std::string source, std::string destination);   

    void DeinitializePhysFS();
    
    //
    bool IsDirectory(const char* file);
    void DiscoverFiles(const char* directory, std::vector<std::string>& vFiles, std::vector<std::string>& vDirs);

    std::string GetUniqueName(const char* path, const char* name);
    void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);
    
    // Duplicate
    bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
    bool DuplicateFile(const char* srcFile, const char* dstFile);
}