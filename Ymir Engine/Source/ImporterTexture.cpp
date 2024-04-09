#include "ImporterTexture.h"
#include "JsonFile.h"
#include "Random.h"
#include "Log.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "PhysfsEncapsule.h"

#include "External/mmgr/mmgr.h"

void ImporterTexture::Import(std::string path, ResourceTexture* ourTexture)
{
	// -1. Set texture properties

	GLenum format;
	ILenum ILformat;

	std::string textureName;
	PhysfsEncapsule::SplitFilePath(path.c_str(), nullptr, &textureName);

	char typeChar = textureName.back();

	switch (typeChar) {

	case 'D':
	{
		ourTexture->type = TextureType::DIFFUSE;
		format = GL_RGBA;
		ILformat = IL_RGBA;
		break;
	}
	case 'S':
	{
		ourTexture->type = TextureType::SPECULAR;
		format = GL_ALPHA;
		ILformat = IL_ALPHA;
		break;
	}
	case 'N':
	{
		ourTexture->type = TextureType::NORMAL;
		format = GL_RGB;
		ILformat = IL_RGB;
		break;
	}
	case 'H':
	{
		ourTexture->type = TextureType::HEIGHT;
		format = GL_LUMINANCE;
		ILformat = IL_LUMINANCE;
		break;
	}
	case 'A':
	{
		ourTexture->type = TextureType::AMBIENT;
		format = GL_LUMINANCE;
		ILformat = IL_LUMINANCE;
		break;
	}
	case 'E':
	{
		ourTexture->type = TextureType::EMISSIVE;
		format = GL_RGBA;
		ILformat = IL_RGBA;
		break;
	}
	default:
	{
		ourTexture->type = TextureType::DIFFUSE;
		format = GL_RGBA;
		ILformat = IL_RGBA;
		break;
	}

	}

	// Andreu: IDK if this should be here
	ourTexture->SetAssetsFilePath(path);

	// 0. Handle UID

	JsonFile* tmpMetaFile = JsonFile::GetJSON(path + ".meta");

	if (tmpMetaFile) {

		// The meta file exists; it's not the first time we load the texture.
		ourTexture->UID = tmpMetaFile->GetInt("UID");

	}
	else {

		// The meta file doesn't exists; first time loading the texture.
		ourTexture->UID = Random::Generate();

		JsonFile textureMetaFile;

		textureMetaFile.SetString("Assets Path", path.c_str());
		textureMetaFile.SetString("Library Path", (External->fileSystem->libraryTexturesPath + std::to_string(ourTexture->UID) + ".dds").c_str());
		textureMetaFile.SetInt("UID", ourTexture->UID);
		textureMetaFile.SetString("Type", "Texture");
		textureMetaFile.SetString("TextureType", ResourceTexture::GetNameFromTextureType(ourTexture->type).c_str());

		External->fileSystem->CreateMetaFileFromAsset(path, textureMetaFile);

	}

	delete tmpMetaFile;

	// 1. Load DevIL Image

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(path.c_str())) {
		// Image loaded successfully
		LOG("Image loaded successfully at %s", path.c_str());
	}
	else {
		// Error loading the image
		LOG("[ERROR] Unable to load image.");
		return;
	}

	// 2. Convert the Image to OpenGL Texture Format

	if (ilConvertImage(ILformat, IL_UNSIGNED_BYTE)) {
		// Image converted successfully
		LOG("Image converted successfully at %s", path.c_str());
	}
	else {
		// Error converting the image
		ILenum error = ilGetError();
		LOG("[ERROR] Could not convert the image: %s\n", iluErrorString(error));
		return;
	}

	// 3. Get Image Data

	ILubyte* imageData = ilGetData();
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	ourTexture->SetWidth(width);
	ourTexture->SetHeight(height);

	ourTexture->ID = ilutGLBindTexImage();

	// 4. Generate OpenGL Texture

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &ourTexture->ID);
	glBindTexture(GL_TEXTURE_2D, ourTexture->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

	// 5. Set Texture Parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	// 6. Save texture on Library as .dds

	std::string libraryPath = External->fileSystem->libraryTexturesPath + std::to_string(ourTexture->UID) + ".dds";

	if (!PhysfsEncapsule::FileExists(libraryPath)) {

		External->fileSystem->SaveTextureToFile(ourTexture, libraryPath);

	}

	// 7. Clean Up

	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &imageID);

}

uint ImporterTexture::Save(const Texture* ourTexture, char** fileBuffer)
{
	return uint();
}

void ImporterTexture::Load(const char* fileBuffer, Texture* ourTexture)
{

}
