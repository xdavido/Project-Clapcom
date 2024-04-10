#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "Log.h"
#include "GameObject.h"
#include "ModuleAudio.h"

#include "External/ImGuizmo/include/ImGuizmo.h"

#include "External/MathGeoLib/include/Math/Quat.h"

#include "External/Optick/include/optick.h"

#include "External/mmgr/mmgr.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	editorCamera = new CCamera(nullptr, false);

	editorCamera->SetPos(-36.0f, 48.00f, 100.0f);
	editorCamera->LookAt(float3(0.f, 0.f, 0.f));
	editorCamera->SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);

	LOG("Creating ModuleCamera3D");

}

ModuleCamera3D::~ModuleCamera3D()
{
	delete editorCamera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

//#ifdef _STANDALONE
//
//	External->audio->PlayEvent(External->scene->gameCameraObject->UID, std::string("JV"));
//	
//#endif // STANDALONE

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	OPTICK_EVENT();

	// TODO: consider moving all camera input to CameraInput function, so that it doesn't work while not hovering

	//if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	//{
	//	External->audio->PlayEvent(External->scene->gameCameraObject->UID, std::string("JV"));
	//}
	//if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
	//{
	//	External->audio->StopEvent(External->scene->gameCameraObject->UID, std::string("JV"));
	//}

	float3 newPos(0, 0, 0);

	float speed = 20.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed *= 6;

	// Mouse wheel Zoom In and Zoom Out handling
	if (hoveringEditor)
	{
		editorCamera->ZoomHandling(newPos, speed);

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {

			// Mouse wheel pressed while dragging movement handling

			editorCamera->PanHandling(newPos, speed, dt);

		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) {

			// WASD Camera Movement Handling

			editorCamera->MovementHandling(newPos, speed);

			// Camera Rotation Handling

			editorCamera->RotationHandling(speed, dt);

		}
	}

	editorCamera->UpdatePos(newPos);

	//if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) {

	//	// Center camera to 0,0,0 when pressing Left Alt
	//	Focus();
	//	//editorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));

	//}


	// Orbit Camera around selectedGO
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && !ImGuizmo::IsUsing())
	{
		if (App->scene->selectedGO != nullptr)
		{
			float distToRef = float3(App->scene->selectedGO->mTransform->GetGlobalPosition() - editorCamera->frustum.pos).Length();

			editorCamera->RotationHandling(speed, App->GetDT());
			editorCamera->frustum.pos = App->scene->selectedGO->mTransform->GetGlobalPosition() + (editorCamera->frustum.front * -distToRef);
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Focus()
{
	float3 center = float3::zero;

	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		CMesh* meshComponent = (CMesh*)(*it)->GetComponent(ComponentType::MESH);

		if (meshComponent!=nullptr)
		{
			if ((*meshComponent).mOwner->selected)
			{
				//Reference = selectedList[i]->mesh->global_aabb.CenterPoint();
				float3 size = meshComponent->globalAABB.Size();
				center = meshComponent->globalAABB.CenterPoint();
				editorCamera->frustum.pos.Set(center.x + size.x, center.y + size.y, center.z + size.z);
				editorCamera->LookAt(meshComponent->globalAABB.CenterPoint());
			}
		}
	}

	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it) {

		// Check if the current game object is selected.
		if ((*it)->selected)
		{
			// TODO: mesh reference doesn't work
			//if ((*it)->GetComponent(ComponentType::MESH) != nullptr)
			//{
			//	// TODO: orbit camera
			//	//Reference = selectedList[i]->mesh->global_aabb.CenterPoint();
			//	float3 size = static_cast<CMesh*>((*it)->GetComponent(ComponentType::MESH))->meshReference->globalAABB.Size();
			//	center = static_cast<CMesh*>((*it)->GetComponent(ComponentType::MESH))->meshReference->globalAABB.CenterPoint();
			//	editorCamera->frustum.pos.Set(center.x + size.x, center.y + size.y, center.z + size.z);
			//	editorCamera->LookAt(static_cast<CMesh*>((*it)->GetComponent(ComponentType::MESH))->meshReference->globalAABB.CenterPoint());
			//}

			if ((*it)->GetComponent(ComponentType::MESH) == nullptr)
			{
				CTransform* transform = (CTransform*)(*it)->mTransform;
				float3 position = transform->GetGlobalPosition();
				//Reference = position;
				editorCamera->frustum.pos.Set(position.x + 5.0f, position.y + 5.0f, position.z + +5.0f);
				editorCamera->LookAt(position);
			}
		}
	}
}

void ModuleCamera3D::CameraInput()
{
	if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE)
	{
		Focus();
	}
}

void ModuleCamera3D::CreateMousePickingRay(float mousePosX_normalized, float mousePosY_normalized)
{
	mousePickingRay = editorCamera->frustum.UnProjectLineSegment(mousePosX_normalized, mousePosY_normalized);

	App->scene->HandleGameObjectSelection(mousePickingRay);
}