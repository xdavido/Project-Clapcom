#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include <string>
#include <vector>
#include <map>

class GameObject;
class Channel;
class CMesh;
class AnimationController;
class Animation;

class CAnimation : public Component {
public:
	CAnimation(GameObject* owner);
	~CAnimation();

	void Start();
	void Update(float dt);

	void SetAnimation(const char* name, float blendTime = 0.0f);
	void SetAnimation(uint index, float blendTime = 0.0f);

	AnimationController* GetAnimation(uint index);

private:
	void UpdateChannelsTransform(const AnimationController* settings, const AnimationController* blend, float blendRatio);
	float3 GetChannelPosition(const Channel& channel, float currentKey, float3 default) const;
	Quat GetChannelRotation(const Channel& channel, float currentKey, Quat default) const;
	float3 GetChannelScale(const Channel& channel, float currentKey, float3 default) const;

	void UpdateMeshAnimation(GameObject* owner);

public:
	GameObject* rootBone = nullptr;

	std::map<std::string, GameObject*> boneMapping;
	uint previous_animation = 0;
	uint current_animation = 0;
	bool isPlaying = false;

private:
	float time = 0.0f;
	float prevAnimTime = 0.0f;
	float blendTime = 0.0f;
	float blendTimeDuration = 0.0f;

};