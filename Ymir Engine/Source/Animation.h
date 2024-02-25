#ifndef __ANIMATION__
#define __ANIMATION__

#include <vector>
#include <string>
#include <map>
#include "Bone.h"
#include "Globals.h"
#include "Model.h"

struct AssimpNodeData {
	float4x4 transformation; 
	std::string name; 
	int childrenCount; 
	std::vector<AssimpNodeData> children; 
};

class Animation {
public:

	Animation();
	Animation(const std::string& animationPath, Model* model);
	~Animation();

	Bone* FindBone(std::string& name); 

	inline float GetTickPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	inline const AssimpNodeData& GetRootNode() { return rootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap(){ return boneInfoMap; }
private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

public:


private:

	float duration;
	float ticksPerSecond;

	std::vector<Bone> bones; 
	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;

};

#endif // __ANIMATION__
