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

	// AABB & OBB

	void InitBoundingBoxes();
	void UpdateBoundingBoxes();
	void RenderBoundingBoxes();

public:

	uint nVertices;
	uint nIndices;

	Mesh* meshReference;
	ResourceMesh* rMeshReference;

	// Bounding Boxes Handling
	AABB globalAABB;
	AABB aabb;
	OBB obb;

	bool initBoundingBoxes = false;

};