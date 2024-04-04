#pragma once
#include "Component.h"
#include "ModulePathfinding.h"

struct NavAgentProperties {
	float speed = 0.0f;
	float angularSpeed = 0.0f;
	float stoppingDistance = 0.0f;

};

class CNavMeshAgent : public Component {
public:

	CNavMeshAgent(GameObject* owner);
	virtual ~CNavMeshAgent();

	void OnInspector() override;


public:

	std::vector<float3> path;
	NavAgentProperties properties;

private:

	NavAgent* selectedNav = nullptr;

};