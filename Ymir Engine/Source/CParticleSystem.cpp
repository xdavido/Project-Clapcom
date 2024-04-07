#include "Component.h"
#include "CParticleSystem.h"
#include "CMesh.h" //Necesary for AABBs
#include "GameObject.h"
#include "ModuleResourceManager.h"

#include "MathGeoLib/include/Math/Quat.h"

#include "imGui/imgui.h"
#include "imGui/backends/imgui_impl_opengl3.h"
#include "imGui/backends/imgui_impl_SDL2.h"

#include "EmitterSetting.h"
#include "ParticleEmitter.h"

//#include "parson-master/parson.h" //For JSONs

CParticleSystem::CParticleSystem()
{
	active = true;
	Enable();
	ctype = ComponentType::PARTICLE;
}

CParticleSystem::CParticleSystem(GameObject* own)
{
	mOwner = own;
	UID = External->resourceManager->GenerateNewUID();
	active = true;
	Enable();
	ctype = ComponentType::PARTICLE;
}


CParticleSystem::~CParticleSystem()
{
	for (auto it = allEmitters.rbegin(); it != allEmitters.rend(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

void CParticleSystem::Update()
{
	//Si la primera vez que se ejecuta
	Update(TimeManager::DeltaTime);
}

bool CParticleSystem::Update(float dt)
{
	bool ret = true;

	//if (localPlay)
	{
		if (dt > 0.00f && (localPlay || TimeManager::gameTimer.GetState() != TimerState::STOPPED)) //Si no esta parado sino que esta paused tambien ha de hacer cosas
		{
			for (unsigned int i = 0; i < allEmitters.size(); ++i)
			{
				allEmitters.at(i)->Update(dt);
			}
		}
		else
		{
			for (unsigned int i = 0; i < allEmitters.size(); ++i)
			{
				allEmitters.at(i)->Reset();
			}
		}
	}
	/*else
	{
		for (unsigned int i = 0; i < allEmitters.size(); ++i)
		{
			allEmitters.at(i)->Reset();
		}
	}*/

	return ret;
}

bool CParticleSystem::GetActive()
{
	return active;
}

ParticleEmitter* CParticleSystem::CreateEmitter()
{
	//Creamos un nuevo emisor
	ParticleEmitter* emisor = new ParticleEmitter(this);
	emisor->UID = External->resourceManager->GenerateNewUID();
	allEmitters.push_back(emisor);
	External->renderer3D->particleEmitters = allEmitters;
	emisor->Init(this);

	return emisor;
}

void CParticleSystem::OnInspector()
{
	std::string butonChar;

	//Give an ID to each colapsing header to be able to have more than one of the same time
	//This must be done due to ImGui using the names as the ids of all menus and things
	//int myPosInComponents = mOwner->GetComponentPosition(this); //TODO TONI: Probably it has another way to do this //ERIC: Nada de esto hacia falta, basta con el UID, si que habra que hacer algo para si hay mas de un particle emmiter

	butonChar.append(butonChar.append(std::to_string(UID)).c_str());

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();
	if (ImGui::CollapsingHeader(("ParticleSystem##" + std::to_string(UID)).c_str(), &exists, flags))
	{
		int treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		int leafFlags = treeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//Activate particles if and only if timer is stopped
		if (TimeManager::gameTimer.GetState() == TimerState::STOPPED)
		{
			std::string playButtonName = localPlay ? "Pause: " : "Play: ";
			if (ImGui::Button(playButtonName.c_str())) {
				if (localPlay)
				{
					Stop();
				}
				else
				{
					Play();
				}
			}
			ImGui::SameLine();
		}

		ImGui::Text("Playback time: %.2f", timer.ReadSec());

		ImGui::Separator();

		//Crear emitter
		if (ImGui::TreeNodeEx((void*)(intptr_t)-1, treeFlags, "Emitters"))
		{
			for (int i = 0; i < allEmitters.size(); i++)
			{
				std::string nameEmitter;
				if (std::strcmp(allEmitters.at(i)->name.c_str(), "") == 0)
				{

					//ImGui::Text("ParticleEmmiter %i", i);
					nameEmitter.append("Particle Emitter ");
					nameEmitter.append(std::to_string(i + 1));
				}
				else
				{
					nameEmitter += allEmitters.at(i)->name;
					nameEmitter.append(" ## ");
					nameEmitter.append(std::to_string(i + 1));

				}
				//std::string nameEmitter;
				////ImGui::Text("ParticleEmmiter %i", i);
				//nameEmitter.append("Particle Emitter ");
				//nameEmitter.append(std::to_string(i+1));

				if (ImGui::TreeNodeEx((void*)(intptr_t)i, (!allEmitters.at(i)->modules.empty()) ? treeFlags : leafFlags, nameEmitter.c_str())) //nameEmitter.c_str()
				{
					auto listModule = allEmitters.at(i)->modules;
					int securityCheckTree = 999;
					for (int j = 0; j < listModule.size(); j++)
					{
						std::string particleModule; //Les opciones
						std::string deleteButton; //Lo creamos aqui fuera para evitar petadas, pero como la ID va por nombre ha de ser un string para diferenciarlos

						switch (listModule.at(j)->type)
						{
						case BASE:
						{
							ImGui::Text(particleModule.append("Base ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							//Positions
							EmitterBase* eBase = (EmitterBase*)listModule.at(j);
							eBase->OnInspector();
							break;
						}
						case SPAWN:
						{
							ImGui::Text(particleModule.append("Spawn ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterSpawner* eSpawner = (EmitterSpawner*)listModule.at(j);
							eSpawner->OnInspector();
							break;
						}
						case POSITION:
						{
							ImGui::Text(particleModule.append("Position ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterPosition* ePosition = (EmitterPosition*)listModule.at(j);
							ePosition->OnInspector();
							break;
						}
						case ROTATION:
						{
							ImGui::Text(particleModule.append("Rotation ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterRotation* eRotation = (EmitterRotation*)listModule.at(j);
							eRotation->OnInspector(); //Todo, porque la rotation aun no existe bien , solo mira a camara
							break;
						}
						case SIZEPARTICLE:
						{
							ImGui::Text(particleModule.append("Scale ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterSize* eSize = (EmitterSize*)listModule.at(j);
							eSize->OnInspector();
							break;
						}
						case COLOR:
						{
							ImGui::Text(particleModule.append("Color ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterColor* eColor = (EmitterColor*)listModule.at(j);
							eColor->OnInspector();

							break;
						}
						case IMAGE:
						{
							ImGui::Text(particleModule.append("Image ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterImage* eImage = (EmitterImage*)listModule.at(j);
							eImage->OnInspector();

							break;
						}
						case SHAPE:
						{
							ImGui::Text(particleModule.append("Shape ##").append(std::to_string(j)).c_str());
							ImGui::SameLine();
							deleteButton.append("Delete ##").append(std::to_string(j));
							if (ImGui::SmallButton(deleteButton.c_str()))
							{
								securityCheckTree = allEmitters.at(i)->DestroyEmitter(j);
							}
							deleteButton.clear();

							EmitterShape* eShape = (EmitterShape*)listModule.at(j);
							eShape->OnInspector();

							break;
						}
						case MAX:
							//Esto existe para que sea generico el recorrer el switch de emitters
							break;
						default:
							break;
						}
					}
					if (!allEmitters.at(i)->modules.empty() || securityCheckTree == 0) //Necesario para que no pete el tree pop
					{
						ImGui::TreePop();
					}
				}
				std::string CEid;
				if (ImGui::CollapsingHeader(CEid.append("Emitter Options ##").append(std::to_string(i)).c_str()))
				{
					for (int k = 0; k < EmiterType::MAX; k++)
					{
						std::string emitterType;

						switch (k)
						{
						case BASE:
							emitterType.assign("Base Emitter");
							break;
						case SPAWN:
							emitterType.assign("Spawn Emitter");
							break;
						case POSITION:
							emitterType.assign("Position Emitter");
							break;
						case ROTATION:
							emitterType.assign("Rotation Emitter");
							break;
						case SIZEPARTICLE:
							emitterType.assign("Scale Emitter");
							break;
						case COLOR:
							emitterType.assign("Color Emitter");
							break;
						case IMAGE:
							emitterType.assign("Image Emitter");
							break;
						case SHAPE:
							emitterType.assign("Shape Emitter");
							break;
						case MAX:
							break;
						default:
							break;
						}
						if (ImGui::Button(emitterType.c_str()))
						{
							allEmitters.at(i)->CreateEmitterSettingByType((EmiterType)k);
						}
					}
					//ImGui::End();
					//ImGui::TreePop();
				}
				std::string inputNameID;
				/*char* textInfo = "";
				ImGui::InputText( inputNameID.append("Emitter Name ##").append(std::to_string(i)).c_str(),textInfo, IM_ARRAYSIZE(textInfo));
				{
					allEmitters.at(i)->name.assign(textInfo);
				}*/
			}

			if (ImGui::Button("Create Particle Emitter"))
			{
				CreateEmitter();
			}

			ImGui::TreePop();
		}
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}

//Save all emitters 
const char* CParticleSystem::SaveMetaEmitters()
{
	std::string filePath;

	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{

		//Crear path
		char* serialized_string = NULL;

		json_object_set_string(root_object, "FilePath", filePath.c_str());
		json_object_set_number(root_object, "NumEmitters", allEmitters.size());
		json_object_set_number(root_object, "OwnerID", this->UID);

		//Create array of all emitters UUID
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();

		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Emitters", new_val);
		//We create the value to assign and insert into the array
		JSON_Value* arrayRecord;
		arrayRecord = json_value_init_object();
		//We call save Emitter in Json for each mesh and we save their UUID
		for (int i = 0; i < allEmitters.size(); i++)
		{
			json_array_append_number(arr, SaveEmmiterJSON(allEmitters.at(i)));
		}


		serialized_string = json_serialize_to_string_pretty(root_value);
		puts(serialized_string);

		//Crear el archivo en assets
		std::string nameMeta;
		//nameMeta += ASSETS; //TODO TONI: Tronquito en Resource (nose hacerlo)
		nameMeta += std::to_string(this->UID);
		nameMeta += ".meta";
		json_serialize_to_file(root_value, nameMeta.c_str());
		json_free_serialized_string(serialized_string);
		json_value_free(root_value);

		return nameMeta.c_str();
	}
}

//Save just one emmiter (ESTO es codigo antiguo, si los prefabs van bien supongo que se puede petar)
uint32_t CParticleSystem::SaveEmmiterJSON(ParticleEmitter* emitter)
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{

		//Crear path
		char* serialized_string = NULL;

		json_object_set_number(root_object, "ModulesSize", emitter->modules.size());
		//json_object_set_number(root_object, "OwnerID", mOwner->UID);

		//Create array of all modules info
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();

		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Settings", new_val);

		//We call save Emitter in Json for each mesh and we save their UUID
		for (int i = 0; i < emitter->modules.size(); i++)
		{
			//Components (son object)
			JSON_Value* modulo;
			modulo = json_value_init_object();
			JSON_Object* child_object = json_value_get_object(modulo);

			json_object_set_number(child_object, "Type", (int)emitter->modules.at(i)->type);

			//Y aqui un switch para crear en funcion del tipo
			switch (emitter->modules.at(i)->type)
			{
			case BASE:
			{
				EmitterBase* eBase = (EmitterBase*)emitter->modules.at(i);

				//Lifetime
				json_object_set_number(child_object, "Lifetime", eBase->particlesLifeTime);
				//InitialPosition
				JSON_Array* arrInitialPos;

				//Position
				JSON_Value* jValuePos = json_value_init_array();
				arrInitialPos = json_value_get_array(jValuePos);

				json_object_dotset_value(child_object, "Position", jValuePos);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.x);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.y);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.z);

				break;
			}
			case SPAWN:
			{
				EmitterSpawner* eSpawn = (EmitterSpawner*)emitter->modules.at(i);

				//Time of num based
				json_object_set_boolean(child_object, "TimeBased", eSpawn->basedTimeSpawn);
				//ParticlesToSpawn
				json_object_set_number(child_object, "NumParticles", eSpawn->numParticlesToSpawn);
				//SpawnSpeed
				json_object_set_number(child_object, "SpawnRatio", eSpawn->spawnRatio);

				break;
			}
			case POSITION:
			{
				EmitterPosition* ePosition = (EmitterPosition*)emitter->modules.at(i);

				//Random
				json_object_set_boolean(child_object, "Random", ePosition->randomized);

				//Directions
				//InitialDirection
				JSON_Array* arrInitialDir1;

				//Position
				JSON_Value* jValueDir1 = json_value_init_array();
				arrInitialDir1 = json_value_get_array(jValueDir1);

				json_object_dotset_value(child_object, "Direction1", jValueDir1);
				json_array_append_number(arrInitialDir1, ePosition->direction1.x);
				json_array_append_number(arrInitialDir1, ePosition->direction1.y);
				json_array_append_number(arrInitialDir1, ePosition->direction1.z);

				ePosition->direction1;
				//SecondDirection
				JSON_Array* arrInitialDir2;

				//Position
				JSON_Value* jValueDir2 = json_value_init_array();
				arrInitialDir2 = json_value_get_array(jValueDir2);

				json_object_dotset_value(child_object, "Direction2", jValueDir2);
				json_array_append_number(arrInitialDir2, ePosition->direction2.x);
				json_array_append_number(arrInitialDir2, ePosition->direction2.y);
				json_array_append_number(arrInitialDir2, ePosition->direction2.z);

				//Accelerate
				json_object_set_boolean(child_object, "Accelerates", ePosition->acceleration);

				//Speed
				json_object_set_number(child_object, "Speed1", ePosition->particleSpeed1);
				json_object_set_number(child_object, "Speed2", ePosition->particleSpeed2);

				break;
			}
			case ROTATION:
			{
				//Doesn't store any value (Don't have different billboarding orientations)

				break;
			}
			case SIZEPARTICLE:
			{
				EmitterSize* eSize = (EmitterSize*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eSize->progresive);

				//Scaling Factor
				json_object_set_number(child_object, "Size1", eSize->sizeMultiplier1);
				json_object_set_number(child_object, "Size2", eSize->sizeMultiplier2);

				//Time
				json_object_set_number(child_object, "TimeStart", eSize->startChange);
				json_object_set_number(child_object, "TimeStop", eSize->stopChange);

				break;
			}
			case COLOR:
			{
				EmitterColor* eColor = (EmitterColor*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eColor->progresive);

				//Colors
				//InitialColor
				JSON_Array* arrInitialColor;

				//Position
				JSON_Value* jValueColor1 = json_value_init_array();
				arrInitialColor = json_value_get_array(jValueColor1);

				json_object_dotset_value(child_object, "Color1", jValueColor1);
				json_array_append_number(arrInitialColor, eColor->color1.r);
				json_array_append_number(arrInitialColor, eColor->color1.g);
				json_array_append_number(arrInitialColor, eColor->color1.b);
				json_array_append_number(arrInitialColor, eColor->color1.a);

				//InitialColor
				JSON_Array* arrFinalColor;

				//Position
				JSON_Value* jValueColor2 = json_value_init_array();
				arrFinalColor = json_value_get_array(jValueColor2);

				json_object_dotset_value(child_object, "Color2", jValueColor2);
				json_array_append_number(arrFinalColor, eColor->color2.r);
				json_array_append_number(arrFinalColor, eColor->color2.g);
				json_array_append_number(arrFinalColor, eColor->color2.b);
				json_array_append_number(arrFinalColor, eColor->color2.a);

				//Time
				json_object_set_number(child_object, "TimeStart", eColor->startChange);
				json_object_set_number(child_object, "TimeStop", eColor->stopChange);

				break;
			}
			case MAX:
				break;
			default:
				break;
			}

			json_array_append_value(arr, modulo);
		}


		//serialized_string = json_serialize_to_string_pretty(root_value);
		//puts(serialized_string);

		////Crear el archivo en assets
		//std::string nameMeta;
		//nameMeta += "Assets/Particles/"; //TODO TONI 
		//nameMeta += std::to_string(UID);
		////nameMeta += PAR; TODO TONI
		//json_serialize_to_file(root_value, nameMeta.c_str());
		//json_free_serialized_string(serialized_string);
		//json_value_free(root_value);
	}
	return UID;
}

//Save emmiter on JSON scene
JSON_Value* CParticleSystem::SaveEmmiterJSON2(ParticleEmitter* emitter)
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	//If there is no failure loading
	if (root_value != nullptr && root_object != nullptr)
	{

		//Crear path
		char* serialized_string = NULL;

		json_object_set_number(root_object, "ModulesSize", emitter->modules.size());
		//json_object_set_number(root_object, "OwnerID", mOwner->UID);

		//Create array of all modules info
		JSON_Array* arr;
		JSON_Value* new_val = json_value_init_array();

		arr = json_value_get_array(new_val);
		json_object_dotset_value(root_object, "Settings", new_val);

		//We call save Emitter in Json for each mesh and we save their UUID
		for (int i = 0; i < emitter->modules.size(); i++)
		{
			//Components (son object)
			JSON_Value* modulo;
			modulo = json_value_init_object();
			JSON_Object* child_object = json_value_get_object(modulo);

			json_object_set_number(child_object, "Type", (int)emitter->modules.at(i)->type);

			//Y aqui un switch para crear en funcion del tipo
			switch (emitter->modules.at(i)->type)
			{
			case BASE:
			{
				EmitterBase* eBase = (EmitterBase*)emitter->modules.at(i);

				//Lifetime
				json_object_set_number(child_object, "Lifetime", eBase->particlesLifeTime);
				//InitialPosition
				JSON_Array* arrInitialPos;

				//Position
				JSON_Value* jValuePos = json_value_init_array();
				arrInitialPos = json_value_get_array(jValuePos);

				json_object_dotset_value(child_object, "Position", jValuePos);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.x);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.y);
				json_array_append_number(arrInitialPos, eBase->emitterOrigin.z);

				break;
			}
			case SPAWN:
			{
				EmitterSpawner* eSpawn = (EmitterSpawner*)emitter->modules.at(i);

				//Time of num based
				json_object_set_boolean(child_object, "TimeBased", eSpawn->basedTimeSpawn);
				//ParticlesToSpawn
				json_object_set_number(child_object, "NumParticles", eSpawn->numParticlesToSpawn);
				//SpawnSpeed
				json_object_set_number(child_object, "SpawnRatio", eSpawn->spawnRatio);

				break;
			}
			case POSITION:
			{
				EmitterPosition* ePosition = (EmitterPosition*)emitter->modules.at(i);

				//Random
				json_object_set_boolean(child_object, "Random", ePosition->randomized);

				//Directions
				//InitialDirection
				JSON_Array* arrInitialDir1;

				//Position
				JSON_Value* jValueDir1 = json_value_init_array();
				arrInitialDir1 = json_value_get_array(jValueDir1);

				json_object_dotset_value(child_object, "Direction1", jValueDir1);
				json_array_append_number(arrInitialDir1, ePosition->direction1.x);
				json_array_append_number(arrInitialDir1, ePosition->direction1.y);
				json_array_append_number(arrInitialDir1, ePosition->direction1.z);

				ePosition->direction1;
				//SecondDirection
				JSON_Array* arrInitialDir2;

				//Position
				JSON_Value* jValueDir2 = json_value_init_array();
				arrInitialDir2 = json_value_get_array(jValueDir2);

				json_object_dotset_value(child_object, "Direction2", jValueDir2);
				json_array_append_number(arrInitialDir2, ePosition->direction2.x);
				json_array_append_number(arrInitialDir2, ePosition->direction2.y);
				json_array_append_number(arrInitialDir2, ePosition->direction2.z);

				//Accelerate
				json_object_set_boolean(child_object, "Accelerates", ePosition->acceleration);

				//Speed
				json_object_set_number(child_object, "Speed1", ePosition->particleSpeed1);
				json_object_set_number(child_object, "Speed2", ePosition->particleSpeed2);

				break;
			}
			case ROTATION:
			{
				//Doesn't store any value (Don't have different billboarding orientations)

				break;
			}
			case SIZEPARTICLE:
			{
				EmitterSize* eSize = (EmitterSize*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eSize->progresive);

				//Scaling Factor
				json_object_set_number(child_object, "Size1", eSize->sizeMultiplier1);
				json_object_set_number(child_object, "Size2", eSize->sizeMultiplier2);

				//Time
				json_object_set_number(child_object, "TimeStart", eSize->startChange);
				json_object_set_number(child_object, "TimeStop", eSize->stopChange);

				break;
			}
			case COLOR:
			{
				EmitterColor* eColor = (EmitterColor*)emitter->modules.at(i);

				//Bool size increases progresivly
				json_object_set_boolean(child_object, "Progressive", eColor->progresive);

				//Colors
				//InitialColor
				JSON_Array* arrInitialColor;

				//Position
				JSON_Value* jValueColor1 = json_value_init_array();
				arrInitialColor = json_value_get_array(jValueColor1);

				json_object_dotset_value(child_object, "Color1", jValueColor1);
				json_array_append_number(arrInitialColor, eColor->color1.r);
				json_array_append_number(arrInitialColor, eColor->color1.g);
				json_array_append_number(arrInitialColor, eColor->color1.b);
				json_array_append_number(arrInitialColor, eColor->color1.a);

				//InitialColor
				JSON_Array* arrFinalColor;

				//Position
				JSON_Value* jValueColor2 = json_value_init_array();
				arrFinalColor = json_value_get_array(jValueColor2);

				json_object_dotset_value(child_object, "Color2", jValueColor2);
				json_array_append_number(arrFinalColor, eColor->color2.r);
				json_array_append_number(arrFinalColor, eColor->color2.g);
				json_array_append_number(arrFinalColor, eColor->color2.b);
				json_array_append_number(arrFinalColor, eColor->color2.a);

				//Time
				json_object_set_number(child_object, "TimeStart", eColor->startChange);
				json_object_set_number(child_object, "TimeStop", eColor->stopChange);

				break;
			}
			case MAX:
				break;
			default:
				break;
			}

			json_array_append_value(arr, modulo);
		}
	}
	return root_value;
}

ParticleEmitter* CParticleSystem::LoadEmitterFromMeta(const char* pathMeta)
{
	ParticleEmitter* pE = new ParticleEmitter(this);
	pE->owner = this;
	JSON_Value* root_value = json_parse_file(pathMeta);
	JSON_Object* root_object = json_value_get_object(root_value);

	int numEmitters = json_object_get_number(root_object, "ModulesSize");

	JSON_Array* arr = json_object_get_array(root_object, "Settings");
	for (int i = 0; i < numEmitters; i++)
	{
		JSON_Object* modulo = json_array_get_object(arr, i);

		EmiterType type = (EmiterType)json_object_get_number(modulo, "Type");

		EmitterSetting* instancia = pE->CreateEmitterSettingByType(type);

		switch (type)
		{
		case BASE:
		{
			EmitterBase* eBase = (EmitterBase*)instancia;
			eBase->particlesLifeTime = (float)json_object_get_number(modulo, "Lifetime");

			//Get position array
			JSON_Array* posArr = json_object_get_array(modulo, "Position");

			//Get elements of position
			float posX = json_array_get_number(posArr, 0);
			float posY = json_array_get_number(posArr, 1);
			float posZ = json_array_get_number(posArr, 2);
			eBase->emitterOrigin = { posX,posY,posZ };

			break;
		}
		case SPAWN:
		{
			EmitterSpawner* eSpawn = (EmitterSpawner*)instancia;

			eSpawn->basedTimeSpawn = json_object_get_boolean(modulo, "TimeBased");
			eSpawn->numParticlesToSpawn = json_object_get_number(modulo, "NumParticles");
			eSpawn->spawnRatio = (float)json_object_get_number(modulo, "SpawnRatio");

			break;
		}
		case POSITION:
		{
			EmitterPosition* ePos = (EmitterPosition*)instancia;

			ePos->randomized = json_object_get_boolean(modulo, "Random");
			ePos->particleSpeed1 = json_object_get_number(modulo, "Speed1");
			ePos->particleSpeed2 = json_object_get_number(modulo, "Speed2");
			ePos->acceleration = json_object_get_boolean(modulo, "Accelerates");

			//Get position array
			JSON_Array* dirArr1 = json_object_get_array(modulo, "Direction1");

			//Get elements of position
			float posX1 = json_array_get_number(dirArr1, 0);
			float posY1 = json_array_get_number(dirArr1, 1);
			float posZ1 = json_array_get_number(dirArr1, 2);
			ePos->direction1 = { posX1,posY1,posZ1 };

			//Get position array
			JSON_Array* dirArr2 = json_object_get_array(modulo, "Direction2");

			//Get elements of position
			float posX2 = json_array_get_number(dirArr2, 0);
			float posY2 = json_array_get_number(dirArr2, 1);
			float posZ2 = json_array_get_number(dirArr2, 2);
			ePos->direction2 = { posX2,posY2,posZ2 };

			break;
		}
		case ROTATION:
		{
			break;
		}
		case SIZEPARTICLE:
		{
			EmitterSize* eSize = (EmitterSize*)instancia;

			eSize->progresive = json_object_get_boolean(modulo, "Progressive");
			eSize->sizeMultiplier1 = (float)json_object_get_number(modulo, "Size1");
			eSize->sizeMultiplier2 = (float)json_object_get_number(modulo, "Size2");
			eSize->startChange = (float)json_object_get_number(modulo, "TimeStart");
			eSize->stopChange = (float)json_object_get_number(modulo, "TimeStop");
			break;
		}
		case COLOR:
		{
			EmitterColor* eColor = (EmitterColor*)instancia;

			eColor->progresive = json_object_get_boolean(modulo, "Progressive");

			eColor->startChange = (float)json_object_get_number(modulo, "TimeStart");
			eColor->stopChange = (float)json_object_get_number(modulo, "TimeStop");

			//Get COLOR array
			JSON_Array* colArr1 = json_object_get_array(modulo, "Color1");

			//Get elements of position
			eColor->color1.r = (float)json_array_get_number(colArr1, 0);
			eColor->color1.g = (float)json_array_get_number(colArr1, 1);
			eColor->color1.b = (float)json_array_get_number(colArr1, 2);
			eColor->color1.a = (float)json_array_get_number(colArr1, 3);

			//Get COLOR array
			JSON_Array* colArr2 = json_object_get_array(modulo, "Color2");

			//Get elements of position
			eColor->color2.r = (float)json_array_get_number(colArr2, 0);
			eColor->color2.g = (float)json_array_get_number(colArr2, 1);
			eColor->color2.b = (float)json_array_get_number(colArr2, 2);
			eColor->color2.a = (float)json_array_get_number(colArr2, 3);

			break;
		}
		case MAX:
			break;
		default:
			break;
		}
	}
	return pE;
}

void CParticleSystem::LoadAllEmmitersJSON(const char* path)
{
	JSON_Value* root_value = json_parse_file(path);
	JSON_Object* root_object = json_value_get_object(root_value);

	int numEmitters = json_object_get_number(root_object, "NumEmitters");

	JSON_Array* arr = json_object_get_array(root_object, "Emitters");
	for (int i = 0; i < numEmitters; i++)
	{
		uint32_t emmiterID = json_array_get_number(arr, i);

		std::string pathEmitter;
		//pathEmitter += PARTICLES_PATH; TODO TONI
		pathEmitter += std::to_string(emmiterID);
		//pathEmitter += PAR; TODO TONI
		this->allEmitters.push_back(LoadEmitterFromMeta(pathEmitter.c_str()));
	}
}

void CParticleSystem::Play()
{
	timer.Start();
	localPlay = true;

	int emittersCount = allEmitters.size();
	for (int i = 0; i < emittersCount; ++i)
	{
		allEmitters[i]->Reset();
	}
}

void CParticleSystem::Stop()
{
	timer.Stop();
	localPlay = false;
}


