#pragma once

#include "Resources.h"

#include "External/DevIL/include/ilu.h"
#include "External/DevIL/include/ilut.h"

#pragma comment( lib, "Source/External/DevIL/libx86/DevIL.lib" )
#pragma comment( lib, "Source/External/DevIL/libx86/ILU.lib" )
#pragma comment( lib, "Source/External/DevIL/libx86/ILUT.lib" )

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

enum class TextureType {

    UNKNOWN = -1,   // NULL Type

    DIFFUSE,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    HEIGHT,
    NORMAL,
    SHININESS,
    OPACITY,
    DISPLACEMENT,
    LIGHTMAP,
    REFLECTION,

    NUM_TYPES       // Total Quantity of Types

};

class ResourceTexture : public Resource {
public:

	ResourceTexture(uint UID);

	bool LoadInMemory() override;
	bool UnloadFromMemory() override;

    void BindTexture(bool bind);

public:

    TextureType type;
    GLuint ID;
    uint UID;

private:

    uint width; 
    uint height; 

    GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

};