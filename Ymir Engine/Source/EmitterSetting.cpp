#include "EmitterSetting.h"
#include "CTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ImporterTexture.h"
#include "ModuleScene.h"

//SHAPES
//#include "EmitterShapeArea.h"

EmitterSetting::EmitterSetting()
{
}

EmitterSetting::~EmitterSetting()
{
}

void EmitterSetting::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterSetting::OnInspector()
{

}

EmitterBase::EmitterBase()
{
	emitterOrigin = float3::zero;
	randomLT = false;
	particlesLifeTime1 = 1.0f;
	particlesLifeTime2 = 2.0f;
	hasDistanceLimit = false;
	distanceLimit = 0.0f;
}

void EmitterBase::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	if (randomLT)
	{
		//Lifetime
		float maxLT;
		float minLT;
		if (particlesLifeTime1 > particlesLifeTime2)
		{
			maxLT = particlesLifeTime1;
			minLT = particlesLifeTime2;
		}
		else
		{
			maxLT = particlesLifeTime2;
			minLT = particlesLifeTime1;
		}

		float randomLT = ((float)rand()) / (float)RAND_MAX;
		float rangeLT = maxLT - minLT;
		float fLT = (randomLT * rangeLT) + minLT;

		particle->oneOverMaxLifetime = 1 / fLT;
	}
	else
	{
		particle->oneOverMaxLifetime = 1 / particlesLifeTime1;
	}

	particle->diesByDistance = hasDistanceLimit;
	particle->distanceLimit = distanceLimit;
	

	CTransform* cTra = (CTransform*)emitter->owner->mOwner->GetComponent(ComponentType::TRANSFORM);
	if (cTra != nullptr) {
		float4x4 matrix = cTra->GetGlobalTransform();
		float3 position;
		Quat rotation;
		float3 escale;
		matrix.Decompose(position, rotation, escale);
		particle->position += position + emitterOrigin; //Se inicializan desde 0,0,0 asi que no deberia haber problema en hacer += pero deberia ser lo mismo.
		particle->worldRotation = rotation;
		particle->size = escale;

		particle->initialPosition = particle->position;
	}
}

void EmitterBase::Update(float dt, ParticleEmitter* emitter)
{
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Actualizamos el tiempo de vida de cada particula
		emitter->listParticles.at(i)->lifetime += emitter->listParticles.at(i)->oneOverMaxLifetime * dt;
	}
}

void EmitterBase::OnInspector()
{
	ImGui::DragFloat3("Initial Pos. ## BASE", &(this->emitterOrigin[0]), 0.1f);
	
	ImGui::Checkbox("Random Lifetime ##BASE", &this->randomLT);
	if (this->randomLT)
	{
		ImGui::DragFloat("Life Time1 ## BASE", &(this->particlesLifeTime1), 0.5F, 0.1F, 720.0F);
		ImGui::DragFloat("Life Time2 ## BASE", &(this->particlesLifeTime2), 0.5F, 0.1F, 720.0F);
	}
	else
	{
		ImGui::DragFloat("Life Time ## BASE", &(this->particlesLifeTime1), 0.5F, 0.5F, 720.0F);
	}

	ImGui::Checkbox("Dies by Distance ##BASE", &this->hasDistanceLimit);
	if(this->hasDistanceLimit)
	{
		ImGui::SliderFloat("Max Distance ##BASE", &(this->distanceLimit), 0.1f, 100.0f);

	}

	//ImGui::Separator();
}

EmitterSpawner::EmitterSpawner()
{
	startMode = ParticlesSpawnEnabeling::PAR_START_NON_STOP;
	playTriggered = false;
	spawnMode = ParticlesSpawnMode::PAR_ONE_PARTICLE_OVER_DELAY;
	spawnRatio = 0.2f; //Dividir en current time por cuantas se spawnean 
	currentTimer = 0.0f;
	numParticlesToSpawn = 5;
	numParticlesForStop = 100;
	numParticlesSpawned = 0;

}

void EmitterSpawner::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterSpawner::Update(float dt, ParticleEmitter* emitter)
{
	bool spawnFromStart = true;
	bool countParticles = false;
	switch (startMode)
	{
	case ParticlesSpawnEnabeling::PAR_START_NON_STOP:
	{
		spawnFromStart = true;
		countParticles = false;
	}
		break;
	case ParticlesSpawnEnabeling::PAR_START_STOP:
	{
		spawnFromStart = true;
		countParticles = true;
	}
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_NON_STOP:
	{
		spawnFromStart = false;
		countParticles = false;
	}
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_STOP:
	{
		spawnFromStart = false;
		countParticles = true;
	}
		break;
	case ParticlesSpawnEnabeling::PAR_ENABLE_MODES_END:
		break;
	default:
		break;
	}

	if((spawnFromStart || playTriggered) && numParticlesSpawned < numParticlesForStop)
	{
		switch (spawnMode)
		{
		case PAR_NUM_PARTICLES_BURST:
		{
			int remainingParticlesToSpawn = numParticlesToSpawn - emitter->listParticles.size();
			if (remainingParticlesToSpawn > 0)
			{
				emitter->SpawnParticle(remainingParticlesToSpawn);
				if (countParticles)
				{
					numParticlesSpawned++;
				}
			}
		}
		
			break;
		case PAR_ONE_PARTICLE_OVER_DELAY:
		{
			currentTimer += dt;
				int numToSpawn = 0;
				if (currentTimer > spawnRatio)
				{
					numToSpawn = currentTimer / spawnRatio;
					emitter->SpawnParticle(numToSpawn);
					if (countParticles)
					{
						numParticlesSpawned++;
					}
				}
				currentTimer -= (spawnRatio * numToSpawn);
		}
			break;
		case PAR_SPAWN_MODE_END:
			break;
		default:
			break;
		}
	}

	//Stop
	if (numParticlesSpawned < numParticlesForStop)
	{
		playTriggered = false;
	}

	
}

bool EmitterSpawner::PlayTrigger(bool val) //Stats play or pause
{
	playTriggered = val;
	numParticlesSpawned = 0;
	return playTriggered;
}

void EmitterSpawner::OnInspector()
{
	int numParticles = this->numParticlesToSpawn;

	//Spawn types
	std::string modeName;
	switch (spawnMode)
	{
	case ParticlesSpawnMode::PAR_NUM_PARTICLES_BURST:
		modeName = "Maximun Particles";
		break;
	case ParticlesSpawnMode::PAR_ONE_PARTICLE_OVER_DELAY:
		modeName = "Spawn after delay";
		break;
	case ParticlesSpawnMode::PAR_SPAWN_MODE_END:
		modeName = "";
		break;
	default:
		break;
	}

	if (ImGui::BeginCombo("##SpawnMode", modeName.c_str()))
	{
		for (int i = 0; i < ParticlesSpawnMode::PAR_SPAWN_MODE_END; i++)
		{
			/*std::string modeName;*/

			switch (ParticlesSpawnMode(i))
			{
			case ParticlesSpawnMode::PAR_NUM_PARTICLES_BURST:
				modeName = "Maximun Particles";
				break;
			case ParticlesSpawnMode::PAR_ONE_PARTICLE_OVER_DELAY:
				modeName = "Spawn after delay";
				break;
			case ParticlesSpawnMode::PAR_SPAWN_MODE_END:
				modeName = "";
				break;
			default:
				break;
			}
			if (ImGui::Selectable(modeName.c_str()))
			{
				spawnMode = (ParticlesSpawnMode)i;
			}
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Text("Spawn Mode");

	switch (spawnMode)
	{
	case ParticlesSpawnMode::PAR_NUM_PARTICLES_BURST:
		if (ImGui::SliderInt("Number Particles ## SPAWN", &numParticles, 0, MAXPARTICLES)) 
		{
			this->numParticlesToSpawn = numParticles; //I thing this was made because it exploded in the original engine if using nPRS directly (Eric)
		}
		break;
	case ParticlesSpawnMode::PAR_ONE_PARTICLE_OVER_DELAY:
		ImGui::SliderFloat("Delay ##SPAWN", &(this->spawnRatio), 0.02f, 1.0f);
		break;
	case ParticlesSpawnMode::PAR_SPAWN_MODE_END:
		modeName = "";
		break;
	default:
		break;
	}

	//Init types (when plays and when it stops)
	std::string modeName2;
	switch (startMode)
	{
	case ParticlesSpawnEnabeling::PAR_START_NON_STOP:
		modeName2 = "Start and NonStop";
		break;
	case ParticlesSpawnEnabeling::PAR_START_STOP:
		modeName2 = "Start with Stop";
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_NON_STOP:
		modeName2 = "Wait then NonStop";
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_STOP:
		modeName2 = "Init with Stop";
		break;
	case ParticlesSpawnEnabeling::PAR_ENABLE_MODES_END:
		break;
	default:
		break;
	}

	if (ImGui::BeginCombo("##InitMode", modeName2.c_str()))
	{
		for (int i = 0; i < ParticlesSpawnEnabeling::PAR_ENABLE_MODES_END; i++)
		{
			/*std::string modeName;*/

			switch (ParticlesSpawnEnabeling(i))
			{
			case ParticlesSpawnEnabeling::PAR_START_NON_STOP:
				modeName2 = "Start and NonStop";
				break;
			case ParticlesSpawnEnabeling::PAR_START_STOP:
				modeName2 = "Start with Stop";
				break;
			case ParticlesSpawnEnabeling::PAR_WAIT_NON_STOP:
				modeName2 = "Wait then NonStop";
				break;
			case ParticlesSpawnEnabeling::PAR_WAIT_STOP:
				modeName2 = "Wait with Stop";
				break;
			case ParticlesSpawnEnabeling::PAR_ENABLE_MODES_END:
				break;
			}
			if (ImGui::Selectable(modeName2.c_str()))
			{
				startMode = (ParticlesSpawnEnabeling)i;
			}
		}

		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Text("Start Mode");

	switch (startMode)
	{
	case ParticlesSpawnEnabeling::PAR_START_NON_STOP:
		//modeName2 = "Start and NonStop";
		break;
	case ParticlesSpawnEnabeling::PAR_START_STOP:
		ImGui::SliderInt("Max Particles until stop ## SPAWN", &numParticlesForStop, 1, MAXPARTICLES);
		ImGui::Text(("Remaining Particles:" + std::to_string(numParticlesForStop - numParticlesSpawned)).c_str());
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_NON_STOP:
		//modeName2 = "Wait then NonStop";
		if (ImGui::Button("Play Trigger"))
		{
			PlayTrigger();
		}
		break;
	case ParticlesSpawnEnabeling::PAR_WAIT_STOP:
		ImGui::SliderInt("Max Particles until stop ## SPAWN", &numParticlesForStop, 1, MAXPARTICLES);
		ImGui::Text(("Remaining Particles:" + std::to_string(numParticlesForStop - numParticlesSpawned)).c_str());
		if (ImGui::Button("Play Trigger"))
		{
			PlayTrigger();
		}
		break;
	case ParticlesSpawnEnabeling::PAR_ENABLE_MODES_END:
		break;
	default:
		break;
	}
	
	
	ImGui::Separator();
}

EmitterPosition::EmitterPosition()
{
	randomized = false; //Si la direccion es solo la uno o un numero random entre la 1 y la 2
	direction1 = { 0,0,0 };
	direction2 = { 0,0,0 };
	acceleration = false;
	particleSpeed1 = 1.0f;
	particleSpeed2 = 0.0f;
	newDirection = { 0,0,0 };
	actualSpeedChange = SpeedChangeMode::PAR_NO_SPEED_CHANGE;
	changeSpeed1 = 0.0f;
	changeSpeed2 = 1.0f;

}

void EmitterPosition::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	if (randomized)
	{
		float3 dir1, dir2;

		dir1 = { direction1.x ,direction1.y ,direction1.z };
		dir2 = { direction2.x ,direction2.y ,direction2.z };

		float maxX, minX;
		if (dir1.x > dir2.x)
		{
			maxX = dir1.x;
			minX = dir2.x;
		}
		else
		{
			maxX = dir2.x;
			minX = dir1.x;
		}

		float maxY, minY;
		if (dir1.y > dir2.y)
		{
			maxY = dir1.y;
			minY = dir2.y;
		}
		else
		{
			maxY = dir2.y;
			minY = dir1.y;
		}

		float maxZ, minZ;
		if (dir1.z > dir2.z)
		{
			maxZ = dir1.z;
			minZ = dir2.z;
		}
		else
		{
			maxZ = dir2.z;
			minZ = dir1.z;
		}

		float randomX = ((float)rand()) / (float)RAND_MAX;
		float rangeX = maxX - minX;
		float fX = (randomX * rangeX) + minX;

		float randomY = ((float)rand()) / (float)RAND_MAX;
		float rangeY = maxY - minY;
		float fY = (randomY * rangeY) + minY;

		float randomZ = ((float)rand()) / (float)RAND_MAX;
		float rangeZ = maxZ - minZ;
		float fZ = (randomZ * rangeZ) + minZ;

		float newModul = GetModuleVec({ fX,fY,fZ });
		particle->velocity.x += fX / newModul;
		particle->velocity.y += fY / newModul;
		particle->velocity.z += fZ / newModul;
	}
	else
	{
		float modul1 = GetModuleVec(direction1);
		if (modul1 > 0)
		{
			particle->velocity.x += direction1.x / modul1;
			particle->velocity.y += direction1.y / modul1;
			particle->velocity.z += direction1.z / modul1;
		}
	}

	particle->velocity.w += particleSpeed1;
}

void EmitterPosition::Update(float dt, ParticleEmitter* emitter)
{

	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Acceleration
		float actualLT = emitter->listParticles.at(i)->lifetime;
		emitter->listParticles.at(i)->velocity.w = particleSpeed1 + ((particleSpeed2 - particleSpeed1) * (actualLT / 1.0f));
		switch (actualSpeedChange)
		{
		case SpeedChangeMode::PAR_NO_SPEED_CHANGE:
		{
			emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
			emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
			emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt; 
		}
			break;
		case SpeedChangeMode::PAR_IF_TIME_ADD:
		{
			if (changeSpeed1<=actualLT && changeSpeed2 >= actualLT)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x + newDirection.x) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y + newDirection.y) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z + newDirection.z) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		case SpeedChangeMode::PAR_ADD_OVER_TIME:
		{

			if (changeSpeed1 <= actualLT && actualLT <= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x + newDirection.x * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y + newDirection.y * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z + newDirection.z * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else if(actualLT >= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x + newDirection.x) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y + newDirection.y) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z + newDirection.z) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		case SpeedChangeMode::PAR_IF_TIME_SUBSTITUTE:
		{
			if (changeSpeed1 <= actualLT && changeSpeed2 >= actualLT)
			{
				emitter->listParticles.at(i)->position.x += newDirection.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += newDirection.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += newDirection.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		case SpeedChangeMode::PAR_SUBSTITUTE_OVER_TIME:
		{
			if (changeSpeed1 <= actualLT && actualLT <= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x * (1- ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.x * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y * (1 - ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.y * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z * (1 - ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.z * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else if (actualLT >= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += newDirection.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += newDirection.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += newDirection.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		default:
			break;
		}
	}
}

void EmitterPosition::OnInspector()
{
	ImGui::Checkbox("Random Movement ##POSITION", &this->randomized);
	if (this->randomized)
	{
		ImGui::DragFloat3("Range 1 ##POSITION", &(this->direction1[0]), 0.1f);
		ImGui::DragFloat3("Range 2 ##POSITION", &(this->direction2[0]), 0.1f);
	}
	else
	{
		ImGui::DragFloat3("Position", &(this->direction1[0]), 0.1f);
	}
	ImGui::Checkbox("Acceleration ##POSITION", &this->acceleration);
	if (this->acceleration)
	{
		ImGui::DragFloat("SpeedInit ##POSITION", &(this->particleSpeed1), 0.2F);
		ImGui::DragFloat("SpeedFinal ##POSITION", &(this->particleSpeed2), 0.2F);
	}
	else
	{
		ImGui::DragFloat("Speed ##POSITION", &(this->particleSpeed1), 0.2F);
	}

	std::string modeName;

	switch (actualSpeedChange)
	{
	case SpeedChangeMode::PAR_NO_SPEED_CHANGE:modeName = "None";break;
	case SpeedChangeMode::PAR_IF_TIME_ADD:modeName = "Add during time";break;
	case SpeedChangeMode::PAR_ADD_OVER_TIME:modeName = "Add over time";break;
	case SpeedChangeMode::PAR_IF_TIME_SUBSTITUTE:modeName = "Change during time";break;
	case SpeedChangeMode::PAR_SUBSTITUTE_OVER_TIME:modeName = "Change over time";break;
	default:break;
	}

	if (ImGui::BeginCombo("##ChangeSpeed", modeName.c_str()))
	{
		for (int i = 0; i < SpeedChangeMode::PAR_SPEED_CHANGE_MODE_END; i++)
		{
			/*std::string modeName;*/

			switch ((SpeedChangeMode)i)
			{
			case SpeedChangeMode::PAR_NO_SPEED_CHANGE:modeName = "None";break;
			case SpeedChangeMode::PAR_IF_TIME_ADD:modeName = "Add during time";break;
			case SpeedChangeMode::PAR_ADD_OVER_TIME:modeName = "Add over time";break;
			case SpeedChangeMode::PAR_IF_TIME_SUBSTITUTE:modeName = "Change during time";break;
			case SpeedChangeMode::PAR_SUBSTITUTE_OVER_TIME:modeName = "Change over time";break;
			default:break;
			}
			if (ImGui::Selectable(modeName.c_str()))
			{
				actualSpeedChange = (SpeedChangeMode)i;
			}
		}

		ImGui::EndCombo();
	}

	switch (actualSpeedChange)
	{
	case SpeedChangeMode::PAR_NO_SPEED_CHANGE:
		//Nothing
		break;
	case SpeedChangeMode::PAR_IF_TIME_ADD:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Adding ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Adding ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case SpeedChangeMode::PAR_ADD_OVER_TIME:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Change ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Change ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case SpeedChangeMode::PAR_IF_TIME_SUBSTITUTE:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Adding ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Adding ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case SpeedChangeMode::PAR_SUBSTITUTE_OVER_TIME:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Change ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Change ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case SpeedChangeMode::PAR_SPEED_CHANGE_MODE_END:
		break;
	default:
		break;
	}

	ImGui::Separator();
}

float EmitterPosition::GetModuleVec(float3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

EmitterRotation::EmitterRotation()
{
	horAlign = true;
	currentAlignmentMode = BillboardType::PAR_LOOK_EDITOR_CAMERA;
}

void EmitterRotation::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterRotation::Update(float dt, ParticleEmitter* emitter)
{
	/*float4x4* camaraMatrix = (float4x4*)cameraSelected->GetViewMatrix().ptr();
	float3 tempPos;
	Quat tempRot;
	float3 tempSca;
	camaraMatrix->Decompose(tempPos, tempRot, tempSca);*/

	switch (currentAlignmentMode)
	{
	case PAR_LOOK_EDITOR_CAMERA:
		EditorCameraAlign();
		break;
	case PAR_LOOK_GAME_CAMERA:
		GameCameraAlign();
		break;
	case PAR_WORLD_ALIGNED:
		WorldAlign();
		break;
	case PAR_AXIS_ALIGNED:
		AxisAlign();
		break;
	case PAR_BILLBOARDING_MODE_END:
		break;
	default:
		break;
	}

	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//emitter->listParticles.at(i)->worldRotation = tempRot;
		emitter->listParticles.at(i)->worldRotation = rotation;
	}

	//ERIC TODO: Las funciones, giros y aligns han de estar aqui, no en el On Inspector
}

void EmitterRotation::OnInspector()
{
	//if (ImGui::Button("EditorCamera"))
	//{
	//	//cameraSelected = External->camera->editorCamera;
	//	viewMatrix = External->camera->editorCamera->GetViewMatrix();
	//}

	//if (ImGui::Button("GameCamera"))
	//{
	//	//cameraSelected = External->scene->gameCameraComponent;
	//	viewMatrix = External->scene->gameCameraComponent->GetViewMatrix();

	//}

	std::string tempAlignment;

	switch (currentAlignmentMode) 
	{
	case BillboardType::PAR_AXIS_ALIGNED: tempAlignment = "Axis Aligned"; break;
		case BillboardType::PAR_LOOK_EDITOR_CAMERA: tempAlignment = "Screen Aligned"; break;
		case BillboardType::PAR_LOOK_GAME_CAMERA: tempAlignment = "Camera Aligned"; break;
		case BillboardType::PAR_WORLD_ALIGNED: tempAlignment = "World Aligned"; break;
		case BillboardType::PAR_BILLBOARDING_MODE_END: tempAlignment = "Not Aligned Yet"; break;
	}
	
	ImGui::Text("Current Billboard: %s", tempAlignment.c_str());

	/*if (ImGui::BeginMenu("Change billboard")) 
	{
		if (ImGui::MenuItem("Editor Camera Aligned")) EditorCameraAlign();
		if (ImGui::MenuItem("Game Camera Aligned")) GameCameraAlign();
		if (ImGui::MenuItem("World Aligned")) WorldAlign();
		if (ImGui::MenuItem("Axis Aligned")) AxisAlign();
		ImGui::EndMenu();
	}*/

	if (ImGui::BeginCombo("##ChangeBillboard", tempAlignment.c_str()))
	{
		for (int i = 0; i < BillboardType::PAR_BILLBOARDING_MODE_END; i++)
		{
			switch ((BillboardType)i)
			{
				case BillboardType::PAR_AXIS_ALIGNED: tempAlignment = "Axis Aligned"; break;
				case BillboardType::PAR_LOOK_EDITOR_CAMERA: tempAlignment = "Screen Aligned"; break;
				case BillboardType::PAR_LOOK_GAME_CAMERA: tempAlignment = "Camera Aligned"; break;
				case BillboardType::PAR_WORLD_ALIGNED: tempAlignment = "World Aligned"; break;
				case BillboardType::PAR_BILLBOARDING_MODE_END: tempAlignment = ""; break;
			}
			if (ImGui::Selectable(tempAlignment.c_str()))
			{
				currentAlignmentMode = (BillboardType)i;
			}
		}

		ImGui::EndCombo();
	}

	switch (currentAlignmentMode)
	{
	case BillboardType::PAR_AXIS_ALIGNED: 
	{
		//Yo aqui pondria un float 3 o algo, idk, revisare como lo hace unity
		//Unity hace algo raro, vertical es que si rota pero solo en el eje Y, horizontal no rota solo mira arriba. La idea seria mas despues que una opcion sea mirar solo dirrecion y otra con el vertical rotar pero en un solo eje
		//Lo unico que no es lo mas util del mundo, se puede dejar para lo ultimo o quiza ni hacerlo, porque con vertical axis fijo tenemos de sobra
		
		std::string tempName = (horAlign ? "Horizontal" : "Vertical");
		ImGui::Text("Current Aligned Axis: %s", tempName.c_str());

		if (ImGui::BeginMenu("Change aligned axis"))
		{
			if (ImGui::MenuItem("Horizontal")) horAlign = true;
			if (ImGui::MenuItem("Vertical")) horAlign = false;

			ImGui::EndMenu();
		}
	}
	break;
	case BillboardType::PAR_LOOK_EDITOR_CAMERA: 
	{
		//Solo texto
	}
	break;
	case BillboardType::PAR_LOOK_GAME_CAMERA: 
	{
		//Solo texto
	}  
	break;
	case BillboardType::PAR_WORLD_ALIGNED: 
	{
		//Ni idea, no entiendo ni que es el world aligment
	}
	break;
	case BillboardType::PAR_BILLBOARDING_MODE_END: break;
	}

	ImGui::Separator();
}

void EmitterRotation::SetRotation(Quat rot)
{
	rotation = rot;
}

void EmitterRotation::EditorCameraAlign()
{ 
	float4x4* camaraMatrix = (float4x4*)External->camera->editorCamera->GetViewMatrix().ptr();
	float3 tempPos;
	Quat tempRot;
	float3 tempSca;
	camaraMatrix->Decompose(tempPos, tempRot, tempSca);
	SetRotation(tempRot);
}

void EmitterRotation::GameCameraAlign()
{
	float4x4* camaraMatrix = (float4x4*)External->scene->gameCameraComponent->GetViewMatrix().ptr();
	float3 tempPos;
	Quat tempRot;
	float3 tempSca;
	camaraMatrix->Decompose(tempPos, tempRot, tempSca);
	SetRotation(tempRot);
}

void EmitterRotation::WorldAlign()
{
	SetRotation(Quat::identity);
}

void EmitterRotation::AxisAlign()
{
	if (horAlign)
	{
		SetRotation(Quat(0.701, 0, 0, -0.713));
	}
	else
	{
		return SetRotation(Quat::identity);
	}

}

EmitterSize::EmitterSize()
{
	originalSize = { 1,1,1 };
	progresive = false;
	startChange = 0.0f; //Range from 0 to 1 as lifetime
	stopChange = 1.0f; //Range from 0 to 1 as lifetime
	sizeMultiplier1 = 1.0f;
	sizeMultiplier2 = 1.0f;
}

void EmitterSize::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	originalSize = particle->size;
}

void EmitterSize::Update(float dt, ParticleEmitter* emitter)
{
	if (progresive)
	{
		float3 base = originalSize;
		float timeForLerp = stopChange - startChange;
		for (int i = 0; i < emitter->listParticles.size(); i++)
		{
			float actualLT = emitter->listParticles.at(i)->lifetime;

			if (startChange <= actualLT && actualLT <= stopChange)
			{
				emitter->listParticles.at(i)->size = base * (sizeMultiplier1 + ((sizeMultiplier2 - sizeMultiplier1) * ((actualLT - startChange) / timeForLerp))); //Lerp size multiplication

			}
		}
	}
}

void EmitterSize::OnInspector()
{
	ImGui::Checkbox("Progresive Scaling", &(this->progresive));
	ImGui::SliderFloat("First Scale", &(this->sizeMultiplier1), 0.1f, 10.0f);
	if (this->progresive)
	{
		ImGui::SliderFloat("End Scale", &(this->sizeMultiplier2), 0.1f, 10.0f);
		ImGui::SliderFloat("Start Change ##SCALE", &(this->startChange), 0.0f, (this->stopChange - 0.05f));
		ImGui::SliderFloat("Stop Change ##SCALE", &(this->stopChange), this->startChange + 0.05f, 1.0f);
	}	
	
	ImGui::Separator();
}

void EmitterColor::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->color = color1;
}

EmitterColor::EmitterColor()
{
	progresive = false;
	startChange = 0.0f; //Range from 0 to 1 as lifetime
	stopChange = 1.0f; //Range from 0 to 1 as lifetime
	color1 = { 0,0,0,1 };
	color2 = { 1,1,1,1 };
}

void EmitterColor::Update(float dt, ParticleEmitter* emitter)
{
	if (progresive)
	{
		float timeForLerp = stopChange - startChange;
		for (int i = 0; i < emitter->listParticles.size(); i++)
		{
			float actualLT = emitter->listParticles.at(i)->lifetime;

			if (startChange <= actualLT && actualLT <= stopChange)
			{
				emitter->listParticles.at(i)->color.r = color1.r + ((color2.r - color1.r) * ((actualLT - startChange) / timeForLerp)); //Lerp red
				emitter->listParticles.at(i)->color.g = color1.g + ((color2.g - color1.g) * ((actualLT - startChange) / timeForLerp)); //Lerp green
				emitter->listParticles.at(i)->color.b = color1.b + ((color2.b - color1.b) * ((actualLT - startChange) / timeForLerp)); //Lerp blue
				emitter->listParticles.at(i)->color.a = color1.a + ((color2.a - color1.a) * ((actualLT - startChange) / timeForLerp)); //Lerp alpha

			}
		}
	}
}

void EmitterColor::OnInspector()
{
	ImGui::Checkbox("Progresive Color", &(this->progresive));
	ImGui::ColorEdit4("First Color", &(this->color1));
	if (this->progresive)
	{
		if (ImGui::ColorEdit4("End Color", &(this->color2)));
		{
			this->color2 = this->color2;
		}
		ImGui::SliderFloat("Initial Color ##COLOR", &(this->startChange), 0.0f, (this->stopChange - 0.05f));
		ImGui::SliderFloat("Final Color ##COLOR", &(this->stopChange), this->startChange + 0.05f, 1.0f);
	}
	
	ImGui::Separator();
}

EmitterImage::EmitterImage(ParticleEmitter* parent)
{
	//TONI: Check if particle system GO has component material
	if (parent->owner->mOwner->GetComponent(ComponentType::MATERIAL))
	{
		mat = (CMaterial*)parent->owner->mOwner->GetComponent(ComponentType::MATERIAL);
	}
	else
	{
		CMaterial* cMat = new CMaterial(parent->owner->mOwner);
		parent->owner->mOwner->AddComponent(cMat);
		mat = cMat;
	}

	rTexTemp = new ResourceTexture();
	imgPath = "Assets/Particles/particleExample.png";
	SetImage(imgPath);
}

void EmitterImage::SetImage(std::string imgPath)
{
	ImporterTexture::Import(imgPath, rTexTemp);
	rTexTemp->type = TextureType::DIFFUSE;
	rTexTemp->UID = Random::Generate();

	mat->diffuse_path = imgPath;
	mat->rTextures.push_back(rTexTemp);
}

void EmitterImage::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->mat = mat;
}

void EmitterImage::Update(float dt, ParticleEmitter* emitter)
{

}

std::vector<std::string> ListFilesInParticlesFolder() {
	std::vector<std::string> files;
	const std::string particlesFolderPath = "Assets/Particles";
	for (const auto& entry : std::filesystem::directory_iterator(particlesFolderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".png") {
			files.push_back(entry.path().filename().string());
		}
	}
	return files;
}

void EmitterImage::OnInspector()
{
	ImGui::Spacing();

	std::vector<std::string> particleFiles = ListFilesInParticlesFolder();
	if (ImGui::BeginCombo("##Texture", rTexTemp->GetAssetsFilePath().c_str()))
	{
		if (!particleFiles.empty()) 
		{
			for (const auto& particleFile : particleFiles) 
			{
				if (ImGui::Selectable(particleFile.c_str())) 
				{
					std::string particlePath = "Assets/Particles/" + particleFile;
					SetImage(particlePath);
				}
			}
		}
		else ImGui::Text("There is no assets.");

		ImGui::EndCombo();
	}

	ImGui::Image((ImTextureID*)rTexTemp->ID, ImVec2(128, 128));

	ImGui::Separator();
}

EmitterShape::EmitterShape(ParticleEmitter* parent)
{
	CTransform* parentTransform = (CTransform*)parent->owner->mOwner->GetComponent(ComponentType::TRANSFORM);
	eTransform = parentTransform;

	shapeParent = parent;
}

void EmitterShape::Spawn(ParticleEmitter* emitter, Particle* particle)
{

}

void EmitterShape::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterShape::OnInspector()
{
	ImGui::Text("Select shape");

	ImGui::Spacing();

	if (ImGui::Button("AREA"))
	{
		CreateSpawnShape(typeShape::AREA);
	}

	if (ImGui::BeginCombo("NONE", "Select Shape", ImGuiComboFlags_PopupAlignLeft))
	{


		ImGui::EndCombo();
	}

	ImGui::Separator();
}

void EmitterShape::CreateSpawnShape(typeShape shape)
{
	EmitterShapeArea* newShape = nullptr;

	switch (shape)
	{
	case typeShape::AREA:
		newShape = new EmitterShapeArea();
		shapeParent->modules.push_back(newShape);
		break;
	case typeShape::CONE:
		//newShape = new EmitterShapeCone();
		break;
	case typeShape::CIRCUMFERENCE:
		//newShape = new EmmiterShapeCircumference();
		break;
	case typeShape::SPHERE:
		//newShape = new EmitterShapeSphere();
		break;
	}
}

EmitterShapeArea::EmitterShapeArea()
{
	angle = 0;
	useDirection = false;
}

void EmitterShapeArea::Spawn(ParticleEmitter* emitter, Particle* particle)
{

	//Get a random spawn point inside of a quad defined by a point and a radius

	float3 localPos;
	localPos.x = offset.x + Random::GenerateRandomFloat(minRange, maxRange);
	localPos.y = offset.y + Random::GenerateRandomFloat(minRange, maxRange);
	localPos.z = offset.z + Random::GenerateRandomFloat(minRange, maxRange);
	particle->position = localPos;

	if (hasInitialSpeed)
	{
		float3 localSpeed = (localPos - float3(offset[0], offset[1], offset[2])).Normalized();
		particle->velocity = float4(float3(emitter->owner->mOwner->mTransform->GetGlobalTransform().TransformDir(localSpeed).Normalized()), speed);	//TONI: estoy delirando

		if (useDirection)
		{
			float3 direction = (localPos - float3(offset[0], offset[1], offset[2]));
			direction = emitter->owner->mOwner->mTransform->GetGlobalTransform().TransformDir(direction).Normalized();

#ifndef STANDALONE
			float4x4 cameraView = External->camera->editorCamera->GetViewMatrix().Transposed();
#else
			float4x4 cameraView = External->moduleRenderer3D->GetGameRenderTarget()->ViewMatrixOpenGL().Transposed();
#endif // !STANDALONE

			direction = cameraView.TransformDir(direction);

			float2 directionViewProj = float2(direction.x, direction.y).Normalized();
			float2 xAxis = float2(1, 0);
			float finalAngle = xAxis.AngleBetween(directionViewProj);
			if (directionViewProj.y < 0)
				finalAngle = 360 * DEGTORAD - finalAngle;
			finalAngle += angle * DEGTORAD;

			//particle->worldRotation = finalAngle;
		}
	}

}

void EmitterShapeArea::Update(float dt, ParticleEmitter* emitter)
{
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Acceleration
		float actualLT = emitter->listParticles.at(i)->lifetime;
		emitter->listParticles.at(i)->velocity.w = speed + ((speed - speed) * (actualLT / 1.0f));

		emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
		emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
		emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
	}
}

void EmitterShapeArea::OnInspector()
{
	std::string suffixLabel = "Dimensions##PaShapeArea";
	ImGui::DragFloat3("Position", &(this->direccion[0]), 0.1f);

	suffixLabel = "Face Direction##PaShapeCone";
	ImGui::Checkbox(suffixLabel.c_str(), &useDirection);

	if (useDirection)
	{
		suffixLabel = "Set Angle##PaShapeCone";
		ImGui::DragFloat(suffixLabel.c_str(), &angle);
	}
}
