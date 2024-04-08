#include "EmitterSetting.h"
#include "CTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ImporterTexture.h"

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
	particlesLifeTime = 1.0f;
}

void EmitterBase::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->oneOverMaxLifetime = 1 / particlesLifeTime;

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
	ImGui::DragFloat("Life Time ## BASE", &(this->particlesLifeTime), 0.5F, 1.0F, 720.0F);
}

EmitterSpawner::EmitterSpawner()
{
	basedTimeSpawn = false;
	spawnRatio = 0.2f; //Dividir en current time por cuantas se spawnean 
	currentTimer = 0.0f;
	numParticlesToSpawn = 5;

}

void EmitterSpawner::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterSpawner::Update(float dt, ParticleEmitter* emitter)
{
	if (!basedTimeSpawn)
	{
		int remainingParticlesToSpawn = numParticlesToSpawn - emitter->listParticles.size();
		if (remainingParticlesToSpawn > 0)
		{
			emitter->SpawnParticle(remainingParticlesToSpawn);
		}
	}
	else
	{
		currentTimer += dt;
		int numToSpawn = 0;
		if (currentTimer > spawnRatio)
		{
			numToSpawn = currentTimer / spawnRatio;
			emitter->SpawnParticle(numToSpawn);
		}
		currentTimer -= (spawnRatio * numToSpawn);
	}
}

void EmitterSpawner::OnInspector()
{
	ImGui::Separator();
	int numParticles = this->numParticlesToSpawn;
	std::string numParticlesWithID = "Particles ##";

	ImGui::Checkbox("(Time / Num) Spawn ", &(this->basedTimeSpawn));

	if (this->basedTimeSpawn)
	{
		if (ImGui::SliderFloat("Delay ##SPAWN", &(this->spawnRatio), 0.1f, 1.0f))
		{

		}
	}
	else
	{
		if (ImGui::SliderInt(numParticlesWithID.append(std::to_string(0/*i*/)).c_str(), &numParticles, 0, MAXPARTICLES)) //ERIC: No recuerdo para que esta en Append de la i aqui, no parece romper nada quitarlo asi que quiza fue una flipada mia inutil del pasados
		{
			this->numParticlesToSpawn = numParticles;
		}
	}
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
	actualSpeedChange = SpeedChangeMode::NONE;
	changeSpeed1 = 0.0f;
	changeSpeed2 = 1.0f;

}

void EmitterPosition::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	if (randomized)
	{
		//X
		float x1 = direction1.x;
		float x2 = direction2.x;
		//Y
		float y1 = direction1.y;
		float y2 = direction2.y;
		//Z
		float z1 = direction1.z;
		float z2 = direction2.z;

		float maxX;
		float minX;
		if (x1 > x2)
		{
			maxX = x1;
			minX = x2;
		}
		else
		{
			maxX = x2;
			minX = x1;
		}

		float maxY;
		float minY;
		if (y1 > y2)
		{
			maxY = y1;
			minY = y2;
		}
		else
		{
			maxY = y2;
			minY = y1;
		}

		float maxZ;
		float minZ;
		if (z1 > z2)
		{
			maxZ = z1;
			minZ = z2;
		}
		else
		{
			maxZ = z2;
			minZ = z1;
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
		case EmitterPosition::NONE:
		{
			emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
			emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
			emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt; 
		}
			break;
		case EmitterPosition::IFTIMEADD:
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
		case EmitterPosition::ADDOVERTIME:
		{

			if (changeSpeed1 <= actualLT && actualLT <= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x + newDirection.x * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y + newDirection.y * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z + newDirection.z * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		case EmitterPosition::IFTIMESUBSTITUTE: 
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
		case EmitterPosition::SUBSTITUTEOVERTIME:
		{
			if (changeSpeed1 <= actualLT && actualLT <= changeSpeed2)
			{
				emitter->listParticles.at(i)->position.x += (emitter->listParticles.at(i)->velocity.x * (1- ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.x * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += (emitter->listParticles.at(i)->velocity.y * (1 - ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.y * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += (emitter->listParticles.at(i)->velocity.z * (1 - ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) + newDirection.z * ((actualLT - changeSpeed1) / (changeSpeed2 - changeSpeed1))) * emitter->listParticles.at(i)->velocity.w * dt;
			}
			else
			{
				emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
				emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
			}
		}
			break;
		case EmitterPosition::MAX:
			break;
		default:
			break;
		}
	}
}

void EmitterPosition::OnInspector()
{
	ImGui::Separator();
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
	ImGui::Checkbox("Acceraltion ##POSITION", &this->acceleration);
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
	case EmitterPosition::NONE:
		modeName = "None";
		break;
	case EmitterPosition::IFTIMEADD:
		modeName = "Add during time";
		break;
	case EmitterPosition::ADDOVERTIME:
		modeName = "Add over time";
		break;
	case EmitterPosition::IFTIMESUBSTITUTE:
		modeName = "Change during time";
		break;
	case EmitterPosition::SUBSTITUTEOVERTIME:
		modeName = "Change over time";
		break;
	case EmitterPosition::MAX:
		modeName = "";
		break;
	default:
		break;
	}

	if (ImGui::BeginCombo("##ChangeSpeed", modeName.c_str()))
	{
		for (int i = 0; i < SpeedChangeMode::MAX; i++)
		{
			/*std::string modeName;*/

			switch ((SpeedChangeMode)i)
			{
			case EmitterPosition::NONE:
				modeName = "None";
				break;
			case EmitterPosition::IFTIMEADD:
				modeName = "Add during time";
				break;
			case EmitterPosition::ADDOVERTIME:
				modeName = "Add over time";
				break;
			case EmitterPosition::IFTIMESUBSTITUTE:
				modeName = "Change during time";
				break;
			case EmitterPosition::SUBSTITUTEOVERTIME:
				modeName = "Change over time";
				break;
			case EmitterPosition::MAX:
				modeName = "";
				break;
			default:
				break;
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
	case EmitterPosition::NONE:
		//Nothing
		break;
	case EmitterPosition::IFTIMEADD:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Adding ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Adding ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case EmitterPosition::ADDOVERTIME:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Change ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Change ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case EmitterPosition::IFTIMESUBSTITUTE:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Adding ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Adding ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case EmitterPosition::SUBSTITUTEOVERTIME:
		ImGui::DragFloat3("New Direction", &(this->newDirection[0]), 0.1f);
		ImGui::SliderFloat("Start Change ##PositionsChange", &(this->changeSpeed1), 0.0f, (this->changeSpeed2 - 0.05f));
		ImGui::SliderFloat("Stop Change ##PositionsChange", &(this->changeSpeed2), this->changeSpeed1 + 0.05f, 1.0f);
		break;
	case EmitterPosition::MAX:
		break;
	default:
		break;
	}
}

float EmitterPosition::GetModuleVec(float3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

void EmitterRotation::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterRotation::Update(float dt, ParticleEmitter* emitter)
{
	float4x4* camaraMatrix = (float4x4*)External->camera->editorCamera->GetViewMatrix().ptr();
	float3 tempPos;
	Quat tempRot;
	float3 tempSca;
	camaraMatrix->Decompose(tempPos, tempRot, tempSca);

	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		emitter->listParticles.at(i)->worldRotation = tempRot;
	}
}

void EmitterRotation::OnInspector()
{
	ImGui::Separator();
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
	ImGui::Separator();
	ImGui::Checkbox("Progresive Scaling", &(this->progresive));
	ImGui::SliderFloat("First Scale", &(this->sizeMultiplier1), 0.1f, 10.0f);
	if (this->progresive)
	{
		ImGui::SliderFloat("End Scale", &(this->sizeMultiplier2), 0.1f, 10.0f);
		ImGui::SliderFloat("Start Change ##SCALE", &(this->startChange), 0.0f, (this->stopChange - 0.05f));
		ImGui::SliderFloat("Stop Change ##SCALE", &(this->stopChange), this->startChange + 0.05f, 1.0f);
	}
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
	ImGui::Separator();
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

	mat->path = imgPath;
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
	ImGui::Separator();

	ImGui::Text("Select particle:");
	
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
	ImGui::Separator();

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
