#include <vector>
#include "Bone.h"
#include "Globals.h"
#include "Log.h"

#include "External/mmgr/mmgr.h"

Bone::Bone()
{

}

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel) : name(name), id(id)
{
	localTransform.identity;
	
	// Manage position keys
	numPositions = channel->mNumPositionKeys; 

	for (int positionIndex = 0; positionIndex < numPositions; positionIndex++) {
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;

		KeyPosition data; 
		data.position.x = aiPosition.x;
		data.position.y = aiPosition.y;
		data.position.z = aiPosition.z;

		data.timeStamp = timeStamp; 
		positions.push_back(data);
	}
	// Manage rotation keys
	numRotations = channel->mNumRotationKeys;

	for (int rotationIndex = 0; rotationIndex < numRotations; rotationIndex++) {
		aiQuaternion aiRotation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;

		KeyRotation data;
		
		data.rotation.w = aiRotation.w; 
		data.rotation.x = aiRotation.x;
		data.rotation.y = aiRotation.y;
		data.rotation.z = aiRotation.z;

		data.timeStamp = timeStamp;
		rotations.push_back(data);
	}

	// Manage scale keys
	numScales = channel->mNumScalingKeys;

	for (int scaleIndex = 0; scaleIndex < numScales; scaleIndex++) {
		aiVector3D aiScales = channel->mScalingKeys[scaleIndex].mValue;
		float timeStamp = channel->mScalingKeys[scaleIndex].mTime;

		KeyScale data;
		data.scale.x = aiScales.x;
		data.scale.y = aiScales.y;
		data.scale.z = aiScales.z;

		data.timeStamp = timeStamp;
		scales.push_back(data);
	}
}

Bone::~Bone()
{

}

void Bone::Update(float animTime)
{
	float4x4 position = InterpolatePosition(animTime);
	float4x4 rotation = InterpolateRotation(animTime);
	float4x4 scale = InterpolateScale(animTime);
	localTransform = position * rotation * scale; 
}

int Bone::GetPositionIndex(float animTime)
{
	for (int i = 0; i < numPositions - 1; i++)
	{
		if (animTime < positions[i + 1].timeStamp)
			return i;
	}
	assert(0);
}

int Bone::GetRotationIndex(float animTime)
{
	for (int i = 0; i < numRotations - 1; i++)
	{
		if (animTime < rotations[i + 1].timeStamp)
			return i;
	}
	assert(0);
}

int Bone::GetScaleIndex(float animTime)
{
	for (int i = 0; i < numScales - 1; i++)
	{
		if (animTime < scales[i + 1].timeStamp)
			return i;
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
	float scaleFactor = 0.0f; 
	float midWayLength = animTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

float4x4 Bone::InterpolatePosition(float animTime)
{
	if (numPositions == 1) {
		return math::TranslateOp(positions[0].position).ToFloat4x4();
	}
	int index = GetPositionIndex(animTime);

	float scaleFactor = GetScaleFactor(positions[index].timeStamp, positions[index + 1].timeStamp, animTime);
	float3 finalPos = positions[index].position.Lerp(positions[index + 1].position, scaleFactor);

	return math::TranslateOp(finalPos).ToFloat4x4();
}

float4x4 Bone::InterpolateRotation(float animTime)
{
	if (numRotations == 1) {
		return rotations[0].rotation.ToFloat4x4();
	}
	
	int index = GetRotationIndex(animTime);
	float scaleFactor = GetScaleFactor(rotations[index].timeStamp, rotations[index + 1].timeStamp, animTime);
	Quat finalRotation = rotations[index].rotation.Slerp(rotations[index + 1].rotation, scaleFactor);

	return finalRotation.ToFloat4x4();
}

float4x4 Bone::InterpolateScale(float animTime)
{
	if (numScales == 1) {
		return math::TranslateOp(scales[0].scale).ToFloat4x4();
	}
	int index = GetScaleIndex(animTime);

	float scaleFactor = GetScaleFactor(scales[index].timeStamp, scales[index + 1].timeStamp, animTime);
	float3 finalScale = scales[index].scale.Lerp(scales[index + 1].scale, scaleFactor);

	return math::TranslateOp(finalScale).ToFloat4x4();
}
