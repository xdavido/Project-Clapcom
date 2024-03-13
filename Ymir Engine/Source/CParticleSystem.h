#pragma once
#pragma once
#ifndef __COMPONENT_PARTICLE_H__
#define __COMPONENT_PARTICLE_H__

#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/quaternion.h"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "ParticleEmitter.h"
#include "CMaterial.h"

#include <vector>
#include <string>

class ParticleEmitter;

class CParticleSystem : public Component
{
public:

	bool active = true;

	CParticleSystem();
	CParticleSystem(GameObject* own, std::string shaderPath = "Assets/Shaders/LavaShader.glsl"); //TODO TONI: CAMBIAR EL SHADER POR UNO PROPIO 

	~CParticleSystem();

	void Update();
	bool Update(float dt);
	bool GetActive();

	//Vector de Emmiters, los emmiters son quienes crean las particulas
	ParticleEmitter* CreateEmitter();

	std::vector<ParticleEmitter*> allEmitters;

	//ImGui
	void OnInspector();

	const char* SaveMetaEmitters(); //Returns the Meta Path File
	uint32_t SaveEmmiterJSON(ParticleEmitter* emitter);
	ParticleEmitter* LoadEmitterFromMeta(const char* pathMeta);
	void LoadAllEmmitersJSON(const char* path);

private:

	CMaterial* mat;
};

#endif //!__COMPONENT_PARTICLE_H__
