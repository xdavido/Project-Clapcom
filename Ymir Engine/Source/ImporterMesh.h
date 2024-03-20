#pragma once

#include "Globals.h"

#include "Mesh.h"
#include "ResourceMesh.h"

#include "External/Assimp/include/mesh.h"

namespace ImporterMesh {

	void Import(const aiMesh* mesh, Mesh* ourMesh); // Import Mesh from Assimp
	const char* Save(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, uint& retSize); // Save Mesh as .ymesh on Library
	void Load(const char* path, ResourceMesh* ourMesh); // Load ResourceMesh from Library
	
}