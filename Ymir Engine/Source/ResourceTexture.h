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
    //SHININESS,
    //OPACITY,
    //DISPLACEMENT,
    //LIGHTMAP,
    //REFLECTION,

    NUM_TYPES       // Total Quantity of Types

};

class ResourceTexture : public Resource {
public:

	ResourceTexture(uint UID = 0);

	bool LoadInMemory() override;
	bool UnloadFromMemory() override;

    void BindTexture(bool bind, GLuint unit);

    // Getters
    uint GetWidth();
    uint GetHeight();

    // Setters
    void SetWidth(uint w);
    void SetHeight(uint h);

    // Checker Texture
    void LoadCheckerImage();

    // Get Sampler Name
    std::string GetSamplerName() const;

    // Get TextureType from Name
    static TextureType GetTextureTypeFromName(const std::string& name);
    static std::string GetNameFromTextureType(const TextureType& type);

public:

    TextureType type;
    GLuint ID;
    uint UID;

private:

    uint width; 
    uint height; 

    GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

};