#include "CAnimation.h"

#include "GameObject.h"
#include "Animation.h"
#include "AnimationController.h"
#include "CTransform.h"
#include "CMesh.h"
#include "TimeManager.h"

CAnimation::CAnimation(GameObject* owner) : Component(owner, ComponentType::ANIMATION)
{

}

CAnimation::~CAnimation()
{

}

void CAnimation::Start() {

}

void CAnimation::Update(float dt) {

}

void CAnimation::SetAnimation(const char* name, float blendTime = 0.0f) {

}

void CAnimation::SetAnimation(uint index, float blendTime = 0.0f) {

}

AnimationController* CAnimation::GetAnimation(uint index) {

}

void CAnimation::UpdateChannelsTransform(const AnimationController* settings, const AnimationController* blend, float blendRatio) {

}

float3 CAnimation::GetChannelPosition(const Channel& channel, float currentKey, float3 default) const {

}

Quat CAnimation::GetChannelRotation(const Channel& channel, float currentKey, Quat default) const {

}

float3 CAnimation::GetChannelScale(const Channel& channel, float currentKey, float3 default) const {

}


void CAnimation::UpdateMeshAnimation(GameObject* owner) {

}