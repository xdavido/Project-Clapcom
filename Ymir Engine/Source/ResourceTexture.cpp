#include "ResourceTexture.h"
#include "Log.h"

#include "External/mmgr/mmgr.h"

ResourceTexture::ResourceTexture(uint UID) : Resource(UID, ResourceType::TEXTURE)
{
	ID = 0;

	type = TextureType::UNKNOWN;

	width = 0;
	height = 0;
}

bool ResourceTexture::LoadInMemory()
{
	bool ret = true;

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

	if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// 5. Set Texture Parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	// 6. Clean Up

	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &imageID);

	return ret;
}

bool ResourceTexture::UnloadFromMemory()
{
	return false;
}

void ResourceTexture::BindTexture(bool bind)
{
	if (bind) {

		glBindTexture(GL_TEXTURE_2D, ID);

	}
	else {

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