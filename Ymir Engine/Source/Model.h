#pragma once

#include <string>
#include <map>

#include "Mesh.h"

#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"

#include "JsonFile.h"

#pragma comment (lib, "Source/External/Assimp/libx86/assimp.lib")

class GameObject;

class Animator;

struct NodeTransform {
  
    float3 translation;
    float3 rotation;
    float3 scale;

};

struct BoneInfo {

    int id;
    float4x4 offset;
};

class Model {

public:

    Model();
    Model(const std::string& path, bool onlyReimport = false, const std::string& shaderPath = SHADER_VS_FS);

    virtual ~Model();

    void LoadModel(const std::string& path, const std::string& shaderPath);
    void DrawModel();

    std::map<std::string, BoneInfo> GetBoneInfoMap() { return boneInfoMap; }
    int GetBoneCount() { return boneCounter; }

private:

    void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parentGO, const std::string& shaderPath, int& iteration);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* linkGO, int rMeshUID, NodeTransform* transform, const std::string& shaderPath);

    void GenerateModelMetaFile();
    void GenerateYmodelFile(const float3& translation, const float3& rotation, const float3& scale);

    void SetVertexBoneDataDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

public:

    // Model Data

    std::string name;
    std::string path;

    std::vector<Mesh> meshes;
    std::string directory;

    GameObject* modelGO;

    // Vector of already loaded textures

    //std::vector<Texture> loadedTextures;

    std::vector<int> embeddedMeshesUID;
    std::vector<int> resourcesUID;

    //Animation stuff
    std::map<std::string, BoneInfo> boneInfoMap;
    int boneCounter;

private:

    bool onlyReimport;
    int processedMeshes; 

};