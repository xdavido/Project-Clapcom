#pragma once
#pragma once
#include "ModulePathFinding.h"
#include "ModuleMonoManager.h"
#include "CNavMeshAgent.h"
#include "CS_Bindings.h"
#include <math.h>
#include <vector>

bool CS_CalculateRandomPath(MonoObject* go, MonoObject* startPos, float radius)
{
	if (External == nullptr || go == nullptr)
		return false;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(go);

	if (comp == nullptr)
		return false;

	comp->path.clear();
	MonoClass* klass = mono_object_get_class(startPos);

	const char* name = mono_class_get_name(klass);
	float3 destination = External->pathFinding->FindRandomPointAround(External->moduleMono->UnboxVector(startPos), radius);
	if (!External->pathFinding->FindPath(External->moduleMono->UnboxVector(startPos), destination, comp->path))
	{
		comp->path.push_back(destination);
	}
	return External->pathFinding->FindPath(External->moduleMono->UnboxVector(startPos), destination, comp->path);
}

bool CS_CalculatePath(MonoObject* go, MonoObject* startPos, MonoObject* endPos)
{
	if (External == nullptr || go == nullptr)
		return false;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(go);

	if (comp == nullptr)
		return false;

	std::vector<float3> possibleVector;

	if (External->pathFinding->FindPath(External->moduleMono->UnboxVector(startPos), External->moduleMono->UnboxVector(endPos), possibleVector))
	{
		comp->path = possibleVector;
		return true;
	}

	return false;
}

bool CS_PathIsPossible(MonoObject* go, MonoObject* startPos, MonoObject* endPos)
{
	if (External == nullptr || go == nullptr)
		return false;

	std::vector<float3> possibleVector;

	if (External->pathFinding->FindPath(External->moduleMono->UnboxVector(startPos), External->moduleMono->UnboxVector(endPos), possibleVector))
		return true;

	return false;
}

int CS_GetPathSize(MonoObject* go)
{
	if (External == nullptr || go == nullptr)
		return 0;

	CNavMeshAgent* agent = CS_CompToComp<CNavMeshAgent*>(go);

	if (agent == nullptr)
		return 0;

	return agent->path.size();
}

MonoObject* CS_GetPointAt(MonoObject* cs_agent, int index)
{
	if (External == nullptr || cs_agent == nullptr)
		return nullptr;

	CNavMeshAgent* agent = CS_CompToComp< CNavMeshAgent*>(cs_agent);

	if (agent == nullptr || agent->path.size() <= index)
		return nullptr;

	float3 position = agent->path[index];

	return External->moduleMono->Float3ToCS(position);
}

void CS_ClearPath(MonoObject* go)
{
	if (External == nullptr || go == nullptr)
		return;

	CNavMeshAgent* agent = CS_CompToComp<CNavMeshAgent*>(go);

	if (agent == nullptr)
		return;

	agent->path.clear();
}

MonoObject* CS_GetDestination(MonoObject* go)
{
	if (External == nullptr || go == nullptr)
		return nullptr;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(go);

	CTransform* trans = comp->mOwner->mTransform;

	if (comp == nullptr)
		return nullptr;

	if (comp->path.size() <= 0)
		return External->moduleMono->Float3ToCS(trans->GetGlobalPosition());

	float3 distance = comp->path.front() - trans->GetGlobalPosition();
	if (Sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z) < comp->properties.stoppingDistance)
	{
		if (comp->path.size() > 1)
			comp->path.erase(comp->path.begin());

		assert(!comp->path.empty());
	}
	return External->moduleMono->Float3ToCS(comp->path.front());
}

MonoObject* CS_GetLastVector(MonoObject* go)
{
	if (External == nullptr || go == nullptr)
		return nullptr;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(go);

	CTransform* trans = comp->mOwner->mTransform;

	if (comp == nullptr)
		return nullptr;

	if (comp->path.size() <= 1)
		return External->moduleMono->Float3ToCS(trans->GetGlobalPosition());


	return External->moduleMono->Float3ToCS(comp->path.back() - comp->path[comp->path.size() - 2]);
}

float CS_GetSpeed(MonoObject* obj)
{
	if (External == nullptr || obj == nullptr)
		return 0.0f;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return 0.0f;

	return comp->properties.speed;
}

float CS_GetAngularSpeed(MonoObject* obj)
{
	if (External == nullptr || obj == nullptr)
		return 0.0f;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return 0.0f;

	return comp->properties.angularSpeed;
}

float CS_GetStoppingDistance(MonoObject* obj)
{
	if (External == nullptr || obj == nullptr)
		return 0.0f;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return 0.0f;

	return comp->properties.stoppingDistance;
}

void CS_SetSpeed(MonoObject* obj, float value)
{
	if (External == nullptr || obj == nullptr)
		return;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return;

	comp->properties.speed = value;
}

void CS_SetAngularSpeed(MonoObject* obj, float value)
{
	if (External == nullptr || obj == nullptr)
		return;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return;

	comp->properties.angularSpeed = value;
}

void CS_SetStoppingDistance(MonoObject* obj, float value)
{
	if (External == nullptr || obj == nullptr)
		return;

	CNavMeshAgent* comp = CS_CompToComp<CNavMeshAgent*>(obj);

	if (comp == nullptr)
		return;

	comp->properties.stoppingDistance = value;
}