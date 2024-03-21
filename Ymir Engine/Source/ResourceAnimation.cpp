#include "ResourceAnimation.h"
#include "ImporterAnimation.h"

#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/Importer.hpp"
#include <functional>
#include "log.h"

ResourceAnimation::ResourceAnimation(uint UID) : Resource(UID, ResourceType::ANIMATION) {
	
	name = "";

	isPlaying = false;
	loop = false;
	pingPong = false;
	backwards = false;
	easeIn = false;
	easeOut = false;
	speed = 1;

	currentTime = 0.0f;
	intensity = 1.0f;

	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	easeInSpeed = 1;
	easeOutSpeed = 1;
	easeInMultiplier = 1.025f;
	easeOutMultiplier = 0.995f;
	duration = 0;

}

bool ResourceAnimation::LoadInMemory()
{
	ImporterAnimation::Load(this->GetLibraryFilePath().c_str(), this);
	LOG("Loaded Animation %s succesfully", name.c_str());
	for (auto it = boneInfoMap.cbegin(); it != boneInfoMap.cend(); it++) {
		LOG("%s", it->first.c_str());
		LOG("%d", it->second.id);
		LOG("%f %f %f %f", it->second.offset.At(0, 0), it->second.offset.At(0, 1), it->second.offset.At(0, 2), it->second.offset.At(0, 3));
		LOG("%f %f %f %f", it->second.offset.At(1, 0), it->second.offset.At(1, 1), it->second.offset.At(1, 2), it->second.offset.At(1, 3));
		LOG("%f %f %f %f", it->second.offset.At(2, 0), it->second.offset.At(2, 1), it->second.offset.At(2, 2), it->second.offset.At(2, 3));
		LOG("%f %f %f %f", it->second.offset.At(3, 0), it->second.offset.At(3, 1), it->second.offset.At(3, 2), it->second.offset.At(3, 3));
	}
	
	return false;
}

bool ResourceAnimation::UnloadFromMemory()
{
	return false;
}

Bone* ResourceAnimation::FindBone(std::string& name)
{
	// iterator 
	std::_Vector_iterator iter = std::find_if(bones.begin(), bones.end(),
		[&](Bone& bone) {
			return bone.GetName() == name;
		});

	if (iter == bones.end()) {
		return nullptr;
	}
	else {
		return &(*iter);
	}
}