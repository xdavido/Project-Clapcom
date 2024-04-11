#include "Animation.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/Importer.hpp"
#include <functional>
#include "log.h"

#include "External/mmgr/mmgr.h"

Animation::Animation() {
	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	easeInSpeed = 1;
	easeOutSpeed = 1;
	easeInMultiplier = 1.025f;
	easeOutMultiplier = 0.995f;
	duration = 0;
}

Animation::Animation(const std::string& animationPath, Model* model, int index)
{
	name = "";
	isPlaying = false;
	loop = false;
	pingPong = false;
	backwards = false;
	easeIn = false;
	easeOut = false;
	speed = 1;
	resetToZero = true;
	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	easeInSpeed = 1;
	easeOutSpeed = 1;
	easeInMultiplier = 1.025f;
	easeOutMultiplier = 0.995f;
	duration = 0;

	ParseAnimationData(animationPath, model, index);
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

	/*LOG("Source matrix:");
	LOG("%f %f %f %f", src->mTransformation.a1, src->mTransformation.a2, src->mTransformation.a3, src->mTransformation.a4);
	LOG("%f %f %f %f", src->mTransformation.b1, src->mTransformation.b2, src->mTransformation.b3, src->mTransformation.b4);
	LOG("%f %f %f %f", src->mTransformation.c1, src->mTransformation.c2, src->mTransformation.c3, src->mTransformation.c4);
	LOG("%f %f %f %f", src->mTransformation.d1, src->mTransformation.d2, src->mTransformation.d3, src->mTransformation.d4);*/

	float4x4 m; 

	m.At(0, 0) = src->mTransformation.a1; m.At(0, 1) = src->mTransformation.a2; m.At(0, 2) = src->mTransformation.a3; m.At(0, 3) = src->mTransformation.a4;
	m.At(1, 0) = src->mTransformation.b1; m.At(1, 1) = src->mTransformation.b2; m.At(1, 2) = src->mTransformation.b3; m.At(1, 3) = src->mTransformation.b4;
	m.At(2, 0) = src->mTransformation.c1; m.At(2, 1) = src->mTransformation.c2; m.At(2, 2) = src->mTransformation.c3; m.At(2, 3) = src->mTransformation.c4;
	m.At(3, 0) = src->mTransformation.d1; m.At(3, 1) = src->mTransformation.d2; m.At(3, 2) = src->mTransformation.d3; m.At(3, 3) = src->mTransformation.d4;

	dest.transformation = m; 

	/*LOG("Destination matrix:");
	LOG("%f %f %f %f", dest.transformation.At(0, 0), dest.transformation.At(0, 1), dest.transformation.At(0, 2), dest.transformation.At(0, 3));
	LOG("%f %f %f %f", dest.transformation.At(1, 0), dest.transformation.At(1, 1), dest.transformation.At(1, 2), dest.transformation.At(1, 3));
	LOG("%f %f %f %f", dest.transformation.At(2, 0), dest.transformation.At(2, 1), dest.transformation.At(2, 2), dest.transformation.At(2, 3));
	LOG("%f %f %f %f", dest.transformation.At(3, 0), dest.transformation.At(3, 1), dest.transformation.At(3, 2), dest.transformation.At(3, 3));*/
	
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++) {
		AssimpNodeData newData; 
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

Animation* Animation::ParseAnimationData(const std::string& animationPath, Model* model, int index)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	aiAnimation* animation = scene->mAnimations[index];
	name = animation->mName.C_Str();
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	ReadHierarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
	return this;
}

