#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include "Model.h"
#include <string>
#include <vector>
#include <map>

class GameObject;
class CMesh;
class Animation;
class Animator;

class CAnimation : public Component {
public:
	CAnimation(GameObject* owner);
	~CAnimation();

	void Update();

	void OnInspector() override;

private:

public:
	//GameObject* rootBone = nullptr;

	//std::map<std::string, GameObject*> boneMapping;

	bool isPlaying = false;
	bool isLoop = false;
	bool isPingPong = false;

private:

	const int animationMaxNum = 3;
	std::string* animationNames = new std::string[animationMaxNum];
	std::vector<Animation> animations;
	int selectedAnimation = 0;
	bool isSelected = false;

};