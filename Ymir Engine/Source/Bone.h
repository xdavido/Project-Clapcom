#pragma once
#include <vector>
#include <map>

#include "External/Assimp/include/scene.h"

#include "External/MathGeoLib/include/Math/float4.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "External/MathGeoLib/include/Math/Quat.h"
#include "External/MathGeoLib/include/Math/TransformOps.h"

struct KeyPosition {
	float3 position;
	float timeStamp; 
};

struct KeyScale {
	float3 scale; 
	float timeStamp; 
};

struct KeyRotation {
	Quat rotation; 
	float timeStamp; 
};

class Bone {
public: 
	Bone(const std::string& name, int id, const aiNodeAnim* channel);
	~Bone();

	void Update(float animTime);

	int GetPositionIndex(float animTime);
	int GetRotationIndex(float animTime);
	int GetScaleIndex(float animTime);

	std::string GetName() { return name; }
	float4x4 GetLocalTransform() { return localTransform; }
	int GetID() { return id; }


private: 
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime);

	float4x4 InterpolatePosition(float animTime);

	float4x4 InterpolateRotation(float animTime);

	float4x4 InterpolateScale(float animTime);

public: 
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;

	std::string name; 

	float4x4 localTransform;
	int id; 

	int numPositions; 
	int numRotations; 
	int numScales; 
};