#ifndef __COMPONENT_MATERIAL__
#define __COMPONENT_MATERIAL__

#include "Component.h"

#include <string>
#include <filesystem>

#include "Globals.h"
#include "ResourceTexture.h"
#include "Shader.h"

class GameObject;

class CMaterial : public Component {
public:

	// Constructor
	CMaterial(GameObject* owner);

	// Destructor
	virtual ~CMaterial();

	// Inherited functions
	void Update() override;
	void OnInspector() override;

	// Function to activate Drag and Drop of textures on Inspector
	void DdsDragDropTarget();

public:

	// Shaders

	int selectedShader;
	std::string shaderPath;
	Shader shader;

	// Textures

	uint UID;
	uint ID;
	std::string path;

	std::vector<ResourceTexture*> rTextures;

};

#endif // __COMPONENT_MATERIAL__