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
	bool shaderDirtyFlag;

	// Textures (need to be reworked to Object Material)

	uint diffuse_UID;
	uint diffuse_ID;
	std::string diffuse_path;

	uint specular_UID;
	uint specular_ID;
	std::string specular_path;

	uint normal_UID;
	uint normal_ID;
	std::string normal_path;

	uint height_UID;
	uint height_ID;
	std::string height_path;

	uint ambient_UID;
	uint ambient_ID;
	std::string ambient_path;

	uint emissive_UID;
	uint emissive_ID;
	std::string emissive_path;

	GameObject* owner;

	std::vector<ResourceTexture*> rTextures;

};

#endif // __COMPONENT_MATERIAL__