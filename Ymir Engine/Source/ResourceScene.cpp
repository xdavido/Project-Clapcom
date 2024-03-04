#include "ResourceScene.h"

#include "External/mmgr/mmgr.h"

ResourceScene::ResourceScene(uint UID) : Resource(UID, ResourceType::SCENE)
{

}

bool ResourceScene::LoadInMemory()
{
    return false;
}

bool ResourceScene::UnloadFromMemory()
{
    return false;
}
