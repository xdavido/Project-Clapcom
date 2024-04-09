#include "ResourceTexture.h"
#include "Log.h"

#include "PhysfsEncapsule.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "External/mmgr/mmgr.h"

ResourceTexture::ResourceTexture(uint UID) : Resource(UID, ResourceType::TEXTURE)
{
	ID = 0;

	this->UID = UID;

	type = TextureType::UNKNOWN;

	width = 0;
	height = 0;
}

bool ResourceTexture::LoadInMemory()
{
	bool ret = true;

	// 0. Set texture properties

	GLenum format;
	ILenum ILformat;

	switch (type)
	{
		case TextureType::DIFFUSE:

			format = GL_RGBA;
			ILformat = IL_RGBA;

			break;

		case TextureType::SPECULAR:

			format = GL_ALPHA;
			ILformat = IL_ALPHA;

			break;

		case TextureType::AMBIENT:

			format = GL_LUMINANCE;
			ILformat = IL_LUMINANCE;

			break;

		case TextureType::EMISSIVE:

			format = GL_RGBA;
			ILformat = IL_RGBA;

			break;

		case TextureType::HEIGHT:

			format = GL_LUMINANCE;
			ILformat = IL_LUMINANCE;

			break;

		case TextureType::NORMAL:

			format = GL_RGB;
			ILformat = IL_RGB;

			break;

	}

	// 1. Load DevIL Image

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(libraryFilePath.c_str())) {
		// Image loaded successfully
		LOG("Image loaded successfully at %s", libraryFilePath.c_str());
	}
	else {
		// Error loading the image
		LOG("[ERROR] Unable to load image.");
		ret = false;
	}

	// 2. Convert the Image to OpenGL Texture Format

	if (ilConvertImage(ILformat, IL_UNSIGNED_BYTE)) {
		// Image converted successfully
		LOG("Image converted successfully at %s", libraryFilePath.c_str());
	}
	else {
		// Error converting the image
		ILenum error = ilGetError();
		LOG("[ERROR] Could not convert the image: %s\n", iluErrorString(error));
		ret = false;
	}

	// 3. Get Image Data

	ILubyte* imageData = ilGetData();
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	ID = ilutGLBindTexImage();

	// 4. Generate OpenGL Texture

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

	// 5. Set Texture Parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	// 6. Save to Library if it isn't there

	//std::string libraryPath = External->fileSystem->libraryTexturesPath + std::to_string(this->UID) + ".dds";

	//if (!PhysfsEncapsule::FileExists(libraryPath)) {

	//	External->fileSystem->SaveTextureToFile(this, libraryPath);

	//}

	// 7. Clean Up

	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &imageID);

	return ret;
}

bool ResourceTexture::UnloadFromMemory()
{
	return false;
}

void ResourceTexture::BindTexture(bool bind, GLuint unit)
{
	if (bind) {

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, ID);

	}
	else {

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}

uint ResourceTexture::GetWidth()
{
	return width;
}

uint ResourceTexture::GetHeight()
{
	return height;
}

void ResourceTexture::SetWidth(uint w)
{
	width = w;
}

void ResourceTexture::SetHeight(uint h)
{
	height = h;
}

void ResourceTexture::LoadCheckerImage()
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {

		for (int j = 0; j < CHECKERS_WIDTH; j++) {

			int c = (((i + j) % 2) == 0) ? 0 : 255;

			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = 0;           
			checkerImage[i][j][2] = (GLubyte)c; 
			checkerImage[i][j][3] = (GLubyte)255; 

		}

	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}

std::string ResourceTexture::GetSamplerName() const
{
	switch (type)
	{
	case TextureType::DIFFUSE:
		return "texture_diffuse";
		break;
	case TextureType::SPECULAR:
		return "texture_specular";
		break;
	case TextureType::AMBIENT:
		return "texture_ambient";
		break;
	case TextureType::EMISSIVE:
		return "texture_emissive";
		break;
	case TextureType::HEIGHT:
		return "texture_height";
		break;
	case TextureType::NORMAL:
		return "texture_normal";
		break;
	default:
		return "texture_diffuse";
		break;
	}
}

TextureType ResourceTexture::GetTextureTypeFromName(const std::string& name)
{
	TextureType type = TextureType::UNKNOWN;

	if (name == "Diffuse") {

		type = TextureType::DIFFUSE;

	}
	else if (name == "Specular") {

		type = TextureType::SPECULAR;

	}
	else if (name == "Normal") {

		type = TextureType::NORMAL;

	}
	else if (name == "Height") {

		type = TextureType::HEIGHT;

	}
	else if (name == "Emissive") {

		type = TextureType::EMISSIVE;

	}
	else if (name == "Ambient") {

		type = TextureType::AMBIENT;

	}

	return type;
}

std::string ResourceTexture::GetNameFromTextureType(const TextureType& type)
{
	std::string name;

	if (type == TextureType::DIFFUSE) {

		name = "Diffuse";

	}
	else if (type == TextureType::SPECULAR) {

		name = "Specular";

	}
	else if (type == TextureType::NORMAL) {

		name = "Normal";

	}
	else if (type == TextureType::HEIGHT) {

		name = "Height";

	}
	else if (type == TextureType::EMISSIVE) {

		name = "Emissive";

	}
	else if (type == TextureType::AMBIENT) {

		name = "Ambient";

	}

	return name;
}
