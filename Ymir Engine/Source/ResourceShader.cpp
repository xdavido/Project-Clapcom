#include "ResourceShader.h"

#include "External/mmgr/mmgr.h"

ResourceShader::ResourceShader(uint UID) : Resource(UID, ResourceType::SHADER)
{

}

bool ResourceShader::LoadInMemory()
{
    return false;
}

bool ResourceShader::UnloadFromMemory()
{
    return false;
}
