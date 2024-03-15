#pragma once
#include <vector>
#include <string>
#include <map>
#include "Globals.h"

#include "Bone.h"
#include "Model.h"

#include "Resources.h"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"


class Model;
struct BoneInfo;

//struct AssimpNodeData {
//	float4x4 transformation;
//	std::string name;
//	int childrenCount;
//	std::vector<AssimpNodeData> children;
//};

class ResourceAnimation : public Resource {
public:

	ResourceAnimation(uint UID);

	bool LoadInMemory() override;
	bool UnloadFromMemory() override;

	float GetSpeed() { return speed; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetDuration(float duration) { this->duration = duration; }
	void SetTickPerSecond(float ticksPerSecond) { this->ticksPerSecond = ticksPerSecond; }
	inline float GetTickPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return boneInfoMap; }

public:

	std::string name;

	bool isPlaying;

	bool loop;

	bool pingPong;

	bool backwards;

	bool easeIn;

	bool easeOut;

	float speed;

	float duration;

	float ticksPerSecond;

	std::vector<Bone> bones;

private:

	std::map<std::string, BoneInfo> boneInfoMap;




};