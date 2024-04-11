#pragma once
#include <vector>
#include <string>
#include <map>
#include "Globals.h"

#include "Bone.h"
#include "Resources.h"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"


class Model;
struct BoneInfo;

struct AssimpNodeData {
	float4x4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class ResourceAnimation : public Resource {
public:

	ResourceAnimation(uint UID);

	bool LoadInMemory() override;
	bool UnloadFromMemory() override;
	Bone* FindBone(std::string& name);
	float GetSpeed() { return speed; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetDuration(float duration) { this->duration = duration; }
	void SetTickPerSecond(float ticksPerSecond) { this->ticksPerSecond = ticksPerSecond; }
	inline float GetTickPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return boneInfoMap; }
	inline const AssimpNodeData& GetRootNode() { return rootNode; }
	 
public:

	std::string name;

	bool isPlaying;

	bool loop;

	bool pingPong;

	bool backwards;

	bool easeIn;

	bool easeOut;

	float speed;

	// If when ending the animation resets currentTime to 0.0f frames
	bool resetToZero;

	std::map<std::string, float> blendMap;

	float duration;

	float ticksPerSecond;

	//float intensity;

	bool backwardsAux;
	bool pingPongAux;
	bool pingPongBackwardsAux;

	float easeInSpeed;
	float easeOutSpeed;
	float easeInMultiplier;
	float easeOutMultiplier;

	float currentTime;

	std::vector<Bone> bones;

	std::map<std::string, BoneInfo> boneInfoMap;

	AssimpNodeData rootNode; 

private:

};