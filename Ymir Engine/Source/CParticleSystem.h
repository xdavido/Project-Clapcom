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

	//Bool to init the particles from inspector
	bool localPlay = false;

	CParticleSystem();
	CParticleSystem(GameObject* own); 

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
	JSON_Value* SaveEmmiterJSON2(ParticleEmitter* emitter); //ERIC: WIP, cambiando el base para otra cosa, que no quiero matar el primer save emitter por si acaso
	ParticleEmitter* LoadEmitterFromMeta(const char* pathMeta);
	void LoadAllEmmitersJSON(const char* path);

	//To play or stop the particles from inspector
	void Stop();
	void Play();

private:

	//Timer for local play
	Timer timer;

};

#endif //!__COMPONENT_PARTICLE_H__
