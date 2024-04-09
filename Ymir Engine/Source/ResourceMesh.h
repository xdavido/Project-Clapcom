#pragma once

#include <vector>

#include "Resources.h"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"

#include "Shader.h"

#include "Mesh.h"

class ResourceMaterial;

class ResourceMesh : public Resource {
public:

	// Constructor & Destructor

	ResourceMesh(uint UID);
	virtual ~ResourceMesh();

	// Inherited Functions

	bool LoadInMemory() override;
	bool UnloadFromMemory() override;

	// Specific Functions

	bool Render();

public:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	ResourceMaterial* material;

private:

	GLuint VBO;
	GLuint EBO;
	GLuint VAO;
};