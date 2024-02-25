#include "Animation.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/Importer.hpp"
#include <functional>

Animation::Animation() {

}

Animation::Animation(const std::string& animationPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	aiAnimation* animation = scene->mAnimations[0];
	duration = animation->mDuration; 
	ticksPerSecond = animation->mTicksPerSecond;
	ReadHierarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Animation::~Animation() {

}

Bone* Animation::FindBone(std::string& name)
{
	// iterator 
	std::_Vector_iterator iter = std::find_if(bones.begin(), bones.end(),
		[&](Bone& bone) {
			return bone.GetName() == name;
		});
	if(iter == bones.end()){
		return nullptr; 
	}
	else {
		return &(*iter);
	}
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	std::map<std::string, BoneInfo>& boneInfoMap = model.GetBoneInfoMap();
	int boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++) {
		aiNodeAnim* channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
	}
	this->boneInfoMap = boneInfoMap; 
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;

	dest.transformation.SetCol(0, float4(src->mTransformation.a1,
		src->mTransformation.b1,
		src->mTransformation.c1,
		src->mTransformation.d1));

	dest.transformation.SetCol(0, float4(src->mTransformation.a2,
		src->mTransformation.b2,
		src->mTransformation.c2,
		src->mTransformation.d2));

	dest.transformation.SetCol(0, float4(src->mTransformation.a3,
		src->mTransformation.b3,
		src->mTransformation.c3,
		src->mTransformation.d3));

	dest.transformation.SetCol(0, float4(src->mTransformation.a4,
		src->mTransformation.b4,
		src->mTransformation.c4,
		src->mTransformation.d4));

	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++) {
		AssimpNodeData newData; 
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}	
}
