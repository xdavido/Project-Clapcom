#include "ResourceModel.h"

#include "External/mmgr/mmgr.h"

ResourceModel::ResourceModel(uint UID) : Resource(UID, ResourceType::MODEL)
{

}

bool ResourceModel::LoadInMemory()
{
    return false;
}

bool ResourceModel::UnloadFromMemory()
{
    return false;
}
