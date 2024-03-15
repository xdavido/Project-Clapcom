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

}

bool ResourceAnimation::LoadInMemory()
{
	ImporterAnimation::Load(this->GetLibraryFilePath().c_str(), this);
	return false;
}

bool ResourceAnimation::UnloadFromMemory()
{
	return false;
}