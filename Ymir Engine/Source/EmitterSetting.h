#pragma once
#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__ 

#include "CParticleSystem.h"
#include "Particle.h"

#include <vector>

enum EmitterType
{
	PAR_BASE,
	PAR_SPAWN,
	PAR_POSITION,
	PAR_ROTATION,
	PAR_SIZE,
	PAR_COLOR,
	PAR_IMAGE,
	PAR_SHAPE,
	PARTICLES_MAX,
};

//EnumS of types of spawn of the spawn setting
enum ParticlesSpawnMode
{
	PAR_NUM_PARTICLES_BURST, //Spawn X particles at a time and doesn't spawn again until that number decreases due to the particles dying
	PAR_ONE_PARTICLE_OVER_DELAY, //Spawn one particle every X seconds (works using delta time).
	PAR_SPAWN_MODE_END
};

enum ParticlesSpawnEnabeling
{
	PAR_START_NON_STOP, //Starts emittng and won´t stop
	PAR_START_STOP, //Starts emitting but stops after X particles spawned
	PAR_WAIT_NON_STOP, //Starts waiting for a Play then won't stop
	PAR_WAIT_STOP, //Starts waiting for a Play and stops after X particles, waiting again
	PAR_ENABLE_MODES_END
};

//Enum of the modes as positions change after spawn
enum SpeedChangeMode
{
	PAR_NO_SPEED_CHANGE,
	PAR_IF_TIME_ADD, //Si en el time añadir el vector a la direccion
	PAR_ADD_OVER_TIME, //Añadir paulatinamente el nuevo vector
	PAR_IF_TIME_SUBSTITUTE, //Si el time cuadra position substituir velocidad
	PAR_SUBSTITUTE_OVER_TIME, //Sobre el tiempo cambia la direccion
	PAR_SPEED_CHANGE_MODE_END,
};

class ParticleEmitter;

//Plantillas para los Emitters, que seran los settings de las particulas
struct EmitterSetting //En Thor Engine esto seria el particle Module
{
public:
	EmitterSetting();
	~EmitterSetting();

	EmitterType type;

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

	bool hasDistanceLimit;
	float distanceLimit;
};

struct EmitterSpawner : EmitterSetting
{
	EmitterSpawner();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	bool PlayTrigger(bool val = true);
	void OnInspector();

	//Variable unica, ritmo de spawn
	ParticlesSpawnMode spawnMode;
	bool playTriggered;
	ParticlesSpawnEnabeling startMode;
	float spawnRatio; //Dividir en current time por cuantas se spawnean 
	float currentTimer;
	int numParticlesToSpawn;
	int numParticlesForStop; //When played, if enabeling mode is stop once it spawn X particles it stops playing
	int numParticlesSpawned;
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

enum class BILLBOARD_TYPE : int
{
	EDITORCAMERA,
	GAMECAMERA,
	WORLDALIGNED,
	AXISALIGNED,
};

struct EmitterRotation : EmitterSetting
{
	EmitterRotation();
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);
	void OnInspector();

	void SetRotation(Quat rot);

	void EditorCameraAlign();
	void GameCameraAlign();
	void WorldAlign();
	void AxisAlign();

	bool horAlign;
	bool verAlign;

	Quat rotation;
	BILLBOARD_TYPE currentAlignment;
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