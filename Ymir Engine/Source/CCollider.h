#pragma once

#include "Component.h"
#include <vector>

class btCollisionShape;

enum ColliderType 
{
	BOX,
	SPHERE,
	CYLINDER,
	CONVEX_HULL,
	MESH_COLLIDER
};

class CCollider : public Component
{
public:
	CCollider(GameObject* owner);
	~CCollider();

	void Update();
	void OnInspector();

	btCollisionShape* GetShape();

	//TODO: funcions per canviar transform del collider, isTrigger, hull collider, mesh collider, etc.

private:
	ColliderType collType;

	btCollisionShape* shape; // Éste es el collider, sólo que se llama shape

	//TODO: crear variables: hull, mesh, sensor, edit collider, transform¿?
};