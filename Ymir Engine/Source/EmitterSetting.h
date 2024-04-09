#pragma once
#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__ 

#include "CParticleSystem.h"
#include "Particle.h"

#include <vector>

enum EmiterType
{
	PARTICLE_BASE,
	PARTICLE_SPAWN,
	PARTICLE_POSITION,
	PARTICLE_ROTATION,
	PARTICLE_SIZE,
	PARTICLE_COLOR,
	PARTICLE_IMAGE,
	SHAPE,
	PARTICLES_MAX,
};

//Enum of the modes as positions change after spawn
enum SpeedChangeMode
{
	NO_SPEED_CHANGE,
	IF_TIME_ADD, //Si en el time añadir el vector a la direccion
	ADD_OVER_TIME, //Añadir paulatinamente el nuevo vector
	IF_TIME_SUBSTITUTE, //Si el time cuadra position substituir velocidad
	SUBSTITUTE_OVER_TIME, //Sobre el tiempo cambia la direccion
	SPEED_CHANGE_MODE_END,
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
	float particlesLifeTime1;
	bool randomLT;
	float particlesLifeTime2;
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
	float3 newDirection;
	float changeSpeed1;
	float changeSpeed2;
	
	SpeedChangeMode actualSpeedChange;

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
	//TONI: Necesito que se le pase un ParticleEmitter para poder acceder al CMaterial del GO
	EmitterImage(ParticleEmitter* parent);
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();
	void SetImage(std::string imgPath);

	ResourceTexture* rTexTemp;
	std::string imgPath;

	//CMaterial que tendrá la particula
	CMaterial* mat;
};

enum class typeShape : int
{
	CIRCUMFERENCE,
	AREA,
	CONE,
	SPHERE,
	NO_SHAPE,
};

struct EmitterShape : EmitterSetting
{
	EmitterShape(ParticleEmitter* parent);

	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	void CreateSpawnShape(typeShape shape);

	EmitterShape* myShape;
	CTransform* eTransform;
	ParticleEmitter* shapeParent;
};

struct EmitterShapeArea : EmitterSetting
{
	EmitterShapeArea();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	float3 offset{ 0,0,0 };
	bool hasInitialSpeed = true;
	float speed;
	float minRange = -1.0f;
	float maxRange = 1.0f;
	float3 direccion;
	float angle;
	bool useDirection = true;
};

//struct EmmiterShapeCircumference : EmitterShape
//{
//	EmmiterShapeCircumference();
//	void Spawn(ParticleEmitter* emitter, Particle* particle);
//	void Update(float dt, ParticleEmitter* emitter);
//	void OnInspector();
//};
//
//struct EmitterShapeCone : EmitterShape
//{
//	EmitterShapeCone();
//	void Spawn(ParticleEmitter* emitter, Particle* particle);
//	void Update(float dt, ParticleEmitter* emitter);
//	void OnInspector();
//};
//
//
//struct EmitterShapeSphere : EmitterShape
//{
//	EmitterShapeSphere();
//	void Spawn(ParticleEmitter* emitter, Particle* particle);
//	void Update(float dt, ParticleEmitter* emitter);
//	void OnInspector();
//};

#endif //__EMITTER_INSTANCE_H__