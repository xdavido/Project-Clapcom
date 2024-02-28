#pragma once

#include "Component.h"
#include <vector>

class btRigidBody;

class CRigidBody : public Component
{
public:
	CRigidBody(GameObject* owner);
	~CRigidBody();

	void Update();
	void OnInspector();

	btRigidBody* GetBody();

	//TODO: funcions per canviar els valors del Cos Rígid

private:
	btRigidBody* body;

	//TODO: crear variables (massa, usegravity, etc.)
};