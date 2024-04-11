#pragma once

#include <vector>
#include <string>
#include <map>
#include "Globals.h"

#include "Bone.h"
#include "Model.h"

class Model;
struct BoneInfo;

class Animation {
public:

	Animation();
	Animation(const std::string& animationPath, Model* model, int index);
	~Animation();

	Bone* FindBone(std::string& name);

	float GetSpeed() { return speed; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetDuration(float duration) { this->duration = duration; }
	void SetTickPerSecond(float ticksPerSecond) { this->ticksPerSecond = ticksPerSecond; }
	float GetTickPerSecond() { return ticksPerSecond; }
	float GetDuration() { return duration; }
	const AssimpNodeData& GetRootNode() { return rootNode; }
	const std::map<std::string, BoneInfo>& GetBoneIDMap(){ return boneInfoMap; }
private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

	Animation* ParseAnimationData(const std::string& animationPath, Model* model, int index);

public:

	std::string name;

	bool isPlaying;

	bool loop;
	
	bool pingPong;

	bool backwards;

	bool easeIn;

	bool easeOut;

	float speed;

	// Dictates the strenght of the animation motion
	// Value 0.0f-1.0f | 1.0f = full motion, 0.0f = no motion
	float intensity;

	float duration;
	float ticksPerSecond;

	bool backwardsAux;
	bool pingPongAux;
	bool pingPongBackwardsAux;

	float easeInSpeed;
	float easeOutSpeed;
	float easeInMultiplier;
	float easeOutMultiplier;

	float currentTime;

	std::vector<Bone> bones;

	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;

private:

};
