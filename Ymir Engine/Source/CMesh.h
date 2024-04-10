#pragma once

#include "Component.h"
#include "Globals.h"

#include "Mesh.h"
#include "ResourceMesh.h"

class CMesh : public Component {
public:

	CMesh(GameObject* owner);
	virtual ~CMesh();

	void Update() override;
	void OnInspector() override;

	void YmeshDragDropTarget();

public:

	uint nVertices;
	uint nIndices;

	Mesh* meshReference;
	ResourceMesh* rMeshReference;

};