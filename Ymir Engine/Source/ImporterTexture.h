#pragma once

#include "Globals.h"

#include "Texture.h"
#include "ResourceTexture.h"

namespace ImporterTexture {

	void Import(std::string path, ResourceTexture* ourTexture); // Import .dds or other formats
	uint Save(const Texture* ourTexture, char** fileBuffer); // Save as .dds on Library
	void Load(const char* fileBuffer, Texture* ourTexture); // Load .dds from Library

}