#include "Globals.h"

#include "ResourceAnimation.h"
#include "Model.h"
#include "External/Assimp/include/anim.h"


namespace ImporterAnimation {

	void Import(const std::string& animationPath, ResourceAnimation* ourAnimation, Model* model, int index); // Import Animation from Assimp
	const char* Save(const ResourceAnimation* ourAnimation, uint& retSize); // Save Animation as .yanim on Library
	void Load(const char* path, ResourceAnimation* ourAnimation); // Load ResourceAnimation from Library

	void SaveAssimpNode(const AssimpNodeData& node, char*& cursor); // Save rootNode recursively
	AssimpNodeData LoadNode(char*& cursor); // Load rootNode recursively
	void CalculateNodeSize(const AssimpNodeData& node, uint& size);

	void ReadMissingBones(const aiAnimation* animation, Model& model, ResourceAnimation* rAnim);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

}