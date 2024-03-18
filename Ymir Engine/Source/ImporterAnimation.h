#include "Globals.h"

#include "Animation.h"
#include "ResourceAnimation.h"

#include "External/Assimp/include/anim.h"

namespace ImporterAnimation {

	void Import(const std::string& animationPath, ResourceAnimation* ourAnimation); // Import Animation from Assimp
	const char* Save(const Animation* ourAnimation, uint& retSize); // Save Animation as .yanim on Library
	void Load(const char* path, ResourceAnimation* ourAnimation); // Load ResourceAnimation from Library

	void SaveAssimpNode(const AssimpNodeData& node, char*& cursor); // Save rootNode recursively
	AssimpNodeData LoadNode(char*& cursor); // Load rootNode recursively
	void CalculateNodeSize(const AssimpNodeData& node, uint& size);

}