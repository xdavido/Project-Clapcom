#pragma once
#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__ 

#include "CParticleSystem.h"
#include "Particle.h"

#include <vector>

enum EmiterType
{
	BASE,
	SPAWN,
	POSITION,
	ROTATION,
	SIZEPARTICLE,
	COLOR,
	IMAGE,
	MAX,
};

class ParticleEmitter;

//Plantillas para los Emitters, que seran los settings de las particulas
struct EmitterSetting //En Thor Engine esto seria el particle Module
{
public:
	EmitterSetting();
	~EmitterSetting();

	EmiterType type;

	virtual void Spawn(ParticleEmitter* emitter, Particle* particle) = 0;
	virtual void Update(float dt, ParticleEmitter* emitter) /*= 0*/;
	virtual void OnInspector();

	bool unique = true; //Si es unique solo puede haber uno de ese tipo
private:

};

struct EmitterBase : EmitterSetting
{
	EmitterBase();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	//Variable unica, posicion donde spawnean
	float3 emitterOrigin;
	float particlesLifeTime;
};

struct EmitterSpawner : EmitterSetting
{
	EmitterSpawner();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	//Variable unica, ritmo de spawn
	bool basedTimeSpawn;
	float spawnRatio; //Dividir en current time por cuantas se spawnean 
	float currentTimer;
	int numParticlesToSpawn;
};

struct EmitterPosition : EmitterSetting
{
	EmitterPosition();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	float GetModuleVec(float3 vec);

	bool randomized; //Si la direccion es solo la uno o un numero random entre la 1 y la 2
	float3 direction1;
	float3 direction2;
	bool acceleration;
	float particleSpeed1;
	float particleSpeed2;
};

struct EmitterRotation : EmitterSetting
{
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();
};

struct EmitterSize : EmitterSetting
{
	EmitterSize();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	float3 originalSize; //Para gestionar si se escala el GameObject
	bool progresive;
	float startChange; //Range from 0 to 1 as lifetime
	float stopChange; //Range from 0 to 1 as lifetime
	float sizeMultiplier1;
	float sizeMultiplier2;


};

struct EmitterColor : EmitterSetting
{
	EmitterColor();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	bool progresive;
	float startChange; //Range from 0 to 1 as lifetime
	float stopChange; //Range from 0 to 1 as lifetime
	Color color1;
	Color color2;
};

struct EmitterImage : EmitterSetting
{
	EmitterImage();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();
	void SetImage(std::string imgPath);

	ResourceTexture* rTexTemp;
	std::string imgPath;
};



#endif //__EMITTER_INSTANCE_H__