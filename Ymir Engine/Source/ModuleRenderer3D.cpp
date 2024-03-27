#include "ModuleRenderer3D.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleResourceManager.h"

#include "Globals.h"
#include "Log.h"
#include "GameObject.h"
#include "G_UI.h"

#include "DefaultShader.h"

#include "External/Optick/include/optick.h"

#include "External/mmgr/mmgr.h"

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	context = nullptr;

	LOG("Creating ModuleRenderer3D");

}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{

}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	// Stream Assimp Log messages to Debug window
	EnableAssimpDebugger();

	// OpenGL initial attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Using OpenGL core profile
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("[ERROR] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		LOG("OpenGL context created sucessfully!");
	}

	// Initializing Glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		LOG("[ERROR] Could not load Glew: %s\n", glewGetErrorString(err));
	}
	else {
		LOG("Successfully using Glew %s", glewGetString(GLEW_VERSION));
	}

	// Initializing DevIL
	ilInit();
	ILenum ILerror = ilGetError();
	if (ILerror != IL_NO_ERROR) {
		LOG("[ERROR] Could not load DevIL: %s\n", iluErrorString(ILerror));
	}
	else {
		LOG("Successfully using DevIL %d", iluGetInteger(IL_VERSION_NUM));
	}

	if (ret == true)
	{
		// Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("[WARNING] Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Could not initialize OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for errors
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Could not initialize OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Could not initialize OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		gl_lights[0].ref = GL_LIGHT0;
		gl_lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		gl_lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		gl_lights[0].SetPos(0.0f, 0.0f, 2.5f);
		gl_lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		// Enable OpenGL initial configurations

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		gl_lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		// Additional OpenGL configurations (starting disabled)

		glDisable(GL_TEXTURE_3D);

		glDisable(GL_MULTISAMPLE);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_POINT_SPRITE);
		glDisable(GL_FOG);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_NORMALIZE);
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Wireframe-Only Mode starting disabled

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	SDL_MaximizeWindow(App->window->window);

	LOG("OpenGL initialized successfully.");

	Grid.axis = true;

	// Load Street Environment from the start

	//models.push_back(Model("Assets/BakerHouse.fbx"));
	//models.push_back(Model("Assets/Street_Environment/StreetEnvironment.fbx"));

	// Skybox

	//models.push_back(Model("Assets/Skybox/Skybox.fbx"));

	// Water Shader

	//models.push_back(Model("Assets/Water.fbx", WATER_SHADER));

	// Shaders already loaded from the start

	Shader* defaultShader = new Shader;
	defaultShader->LoadShader("Assets/Shaders/DefaultShader.glsl");
	delete defaultShader;

	Shader* rainbowShader = new Shader;
	rainbowShader->LoadShader("Assets/Shaders/RainbowShader.glsl");
	delete rainbowShader;

	Shader* lavaShader = new Shader;
	lavaShader->LoadShader("Assets/Shaders/LavaShader.glsl");
	delete lavaShader;

	Shader* colorShader = new Shader;
	colorShader->LoadShader("Assets/Shaders/ColorShader.glsl");
	delete colorShader;

	Shader* waterShader = new Shader;
	waterShader->LoadShader("Assets/Shaders/WaterShader.glsl");
	delete waterShader;

	Shader* animationShader = new Shader;
	animationShader->LoadShader("Assets/Shaders/AnimationShader.glsl");
	delete animationShader;

	Shader* lightingShader = new Shader;
	lightingShader->LoadShader("Assets/Shaders/Lighting Shader.glsl");
	delete lightingShader;

	// Load Editor and Game FrameBuffers

	App->camera->editorCamera->framebuffer.Load();

	App->scene->gameCameraComponent = new CCamera(App->scene->gameCameraObject);

	// TODO: remove and do with proper constructor
	App->scene->gameCameraObject->mTransform->SetPosition(float3(-40.0f, 29.0f, 54.0f));
	App->scene->gameCameraObject->mTransform->SetRotation(float3(180.0f, 40.0f, 180.0f));

	//App->scene->gameCameraComponent->SetPos(-40.0f, 29.0f, 54.0f);
	//App->scene->gameCameraComponent->LookAt(float3(0.f, 0.f, 0.f));

	//Hardcodeado para la VS1
	App->scene->gameCameraComponent->SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);
	App->scene->gameCameraObject->AddComponent(App->scene->gameCameraComponent);
	
	//App->scene->gameCameraComponent->framebuffer.Load();

	defaultFont = new Font("de-valencia-beta.otf", "Assets\\Fonts");

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	OPTICK_EVENT();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->editorCamera->GetViewMatrix().ptr());

	// light 0 on cam pos
	gl_lights[0].SetPos(App->camera->editorCamera->GetPos().x, App->camera->editorCamera->GetPos().y, App->camera->editorCamera->GetPos().z);

	for (uint i = 0; i < MAX_GL_LIGHTS; ++i) 
	{
		gl_lights[i].Render();
	}

	App->editor->AddFPS(App->GetFPS());
	App->editor->AddDT(App->GetDT());
	App->editor->AddMS(App->GetMS());

	return UPDATE_CONTINUE;
}
static bool started = false; // Sorry

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	OPTICK_EVENT();

#ifdef _STANDALONE // Sorry for doing this, it was necessary (Francesc) :(

	if (!started) {

		TimeManager::gameTimer.Start();

		started = true;

	}

#endif // _STANDALONE

	// Clear color buffer and depth buffer before each PostUpdate call
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Your rendering code here

	// --------------------------- Editor Camera FrameBuffer -----------------------------------

#ifndef _STANDALONE

	App->camera->editorCamera->framebuffer.Render(true);

	// Render Grid

	App->camera->editorCamera->Update();

	if (showGrid) {
		
		Grid.Render();

	}

	if (App->scene->gameCameraComponent != nullptr)
	{
		// Render Frustum Box

		if (App->scene->gameCameraObject->active) {

			App->scene->gameCameraComponent->DrawFrustumBox();

		}

		DrawGameObjects();

		DrawUIElements(false, false);

		// Render Bounding Boxes

		if (External->scene->gameCameraComponent->drawBoundingBoxes)
		{
			DrawBoundingBoxes();
		}

		// Render Physics Colliders

		if (App->physics->debugScene)
		{
			DrawPhysicsColliders();
		}

		DrawGameObjects();

		DrawLightsDebug();

		DrawUIElements(false,false);

	}

	App->camera->editorCamera->framebuffer.Render(false);

	// --------------------------- Game Camera FrameBuffer -----------------------------------

	if (App->scene->gameCameraComponent != nullptr)
	{
		App->scene->gameCameraComponent->framebuffer.Render(true);

		App->scene->gameCameraComponent->Update();

		if (App->scene->gameCameraObject->active) {

			if (App->scene->godMode)
			{
				DrawPhysicsColliders();
			}

			DrawGameObjects();

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, App->editor->gameViewSize.x, App->editor->gameViewSize.y, 0.0, 1.0, -1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			DrawUIElements(true,false);

		}

		App->scene->gameCameraComponent->framebuffer.Render(false);

	}

	// --------------------------- Drawing editor and Swaping Window -------------------------

	App->editor->DrawEditor();

#else

	if (App->scene->gameCameraComponent != nullptr)
	{
		App->scene->gameCameraComponent->framebuffer.Render(true);

		App->scene->gameCameraComponent->Update();

		if (App->scene->gameCameraObject->active) {

			if (App->scene->godMode)
			{
				DrawPhysicsColliders();
			}

			DrawGameObjects();

			DrawUIElements(true, true);

		}

		App->scene->gameCameraComponent->framebuffer.Render(false);

		// Adjust FrameBuffer for Standalone

		App->scene->gameCameraComponent->framebuffer.RenderToScreen();

	}

#endif // !_STANDALONE

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	// Clean Framebuffers
	App->camera->editorCamera->framebuffer.Delete();

	// Detach Assimp Log Stream
	CleanUpAssimpDebugger();

	// Shutdown DevIL
	ilShutDown();

	// Delete OpenGL context
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	//for (auto& it = App->scene->cameras.begin(); it != App->scene->cameras.end(); ++it) {

	//	(*it)->SetAspectRatio((float)width / (float)height);

	//}

	App->camera->editorCamera->SetAspectRatio((float)width / (float)height);
	App->scene->gameCameraComponent->SetAspectRatio((float)width / (float)height);
}

void ModuleRenderer3D::SetGameCamera(CCamera* cam)
{
	if (cam != nullptr)
	{
		App->scene->gameCameraComponent = cam;
		OnResize(SDL_GetWindowSurface(App->window->window)->w, SDL_GetWindowSurface(App->window->window)->h);
	}
}

void ModuleRenderer3D::HandleDragAndDrop()
{
	/*if (App->input->droppedFile) {

		if (IsFileExtension(App->input->droppedFileDirectory, ".fbx") || IsFileExtension(App->input->droppedFileDirectory, ".FBX") || IsFileExtension(App->input->droppedFileDirectory, ".DAE") || IsFileExtension(App->input->droppedFileDirectory, ".dae")) {

			models.push_back(Model(App->input->droppedFileDirectory));

			ReloadTextures();

		}
		else if (IsFileExtension(App->input->droppedFileDirectory, ".png") || IsFileExtension(App->input->droppedFileDirectory, ".dds") || IsFileExtension(App->input->droppedFileDirectory, ".tga")) {

			ClearActualTexture();

			for (auto it = models.begin(); it != models.end(); ++it) {

				for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

					if ((*jt).meshGO->selected || (*it).modelGO->selected) {

						(*jt).loadedTextures = false;
						(*jt).loadedShader = false;

						Texture tmpTexture;

						tmpTexture.path = App->input->droppedFileDirectory;

						(*jt).textures.push_back(tmpTexture);

					}

				}

			}

		}

		App->input->droppedFile = false;

	}*/
}

bool ModuleRenderer3D::IsFileExtension(const char* directory, const char* extension)
{
	size_t strLen = strlen(directory);
	size_t suffixLen = strlen(extension);

	if (strLen < suffixLen) {

		return false;
	}

	return strncmp(directory + strLen - suffixLen, extension, suffixLen) == 0;
}

void ModuleRenderer3D::ApplyCheckerTexture()
{
	ClearActualTexture();

	for (auto it = models.begin(); it != models.end(); ++it) {

		for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

			if ((*jt).meshGO->selected || (*it).modelGO->selected) {

				(*jt).loadedTextures = false;
				(*jt).loadedShader = false;

				Texture checkerTexture;

				(*jt).textures.push_back(checkerTexture);

				(*jt).applyCheckerTexture = true;

			}

		}

	}

}

void ModuleRenderer3D::ClearActualTexture()
{
	for (auto it = models.begin(); it != models.end(); ++it) {

		for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

			if ((*jt).meshGO->selected || (*it).modelGO->selected) {

				(*jt).textures.clear();

			}

		}

	}

}

void ModuleRenderer3D::ReloadTextures()
{
	for (auto it = models.begin(); it != models.end(); ++it) {

		for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

			(*jt).loadedTextures = false;
			(*jt).loadedShader = false;

		}

	}
}

void ModuleRenderer3D::DrawBox(float3* vertices, float3 color)
{
	uint indices[24] = {

		0,2,2,
		6,6,4,
		4,0,0,
		1,1,3,
		3,2,4,
		5,6,7,
		5,7,3,
		7,1,5

	};

	glLineWidth(2.f);
	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); i++) {

		glVertex3fv(vertices[indices[i]].ptr());

	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();
	glLineWidth(1.f);
}

void ModuleRenderer3D::DrawBoundingBoxes()
{
	/*for (auto it = models.begin(); it != models.end(); ++it) {

		for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

			if (External->renderer3D->IsInsideFrustum(External->scene->gameCameraComponent, (*jt).globalAABB)) {

				(*jt).UpdateBoundingBoxes();
				(*jt).RenderBoundingBoxes();

			}

		}

	}*/

	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		CMesh* meshComponent = (CMesh*)(*it)->GetComponent(ComponentType::MESH);

		if (meshComponent != nullptr) {

			if (IsInsideFrustum(External->scene->gameCameraComponent, meshComponent->rMeshReference->globalAABB))
			{
				//meshComponent->rMeshReference->UpdateBoundingBoxes();
				meshComponent->rMeshReference->RenderBoundingBoxes();
			}
		}

	}

}

void ModuleRenderer3D::DrawPhysicsColliders()
{
	if (App->editor->gl_Lighting) glDisable(GL_LIGHTING); // Los colliders y los sensores se ver�n siempre sin importar la iluminacion

	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		CCollider* colliderComponent = (CCollider*)(*it)->GetComponent(ComponentType::PHYSICS);

		if (colliderComponent != nullptr && colliderComponent->physBody->drawShape) 
		{
			Color color;
			if (colliderComponent->isSensor) color = App->physics->sensorColor;
			else color = App->physics->colliderColor;

			App->physics->world->debugDrawWorld();

			btCollisionShape* shape = colliderComponent->physBody->body->getCollisionShape();

			glColor3f(color.r, color.g, color.b); // Cambio aqu� el color para tenerlo m�s controlado
			glLineWidth(App->physics->shapeLineWidth); // Lo mismo con la lineWidth

			switch (shape->getShapeType())
			{
			case BroadphaseNativeTypes::BOX_SHAPE_PROXYTYPE: // RENDER BOX ==========================
				App->physics->RenderBoxCollider(colliderComponent->physBody);
				break;
			case BroadphaseNativeTypes::SPHERE_SHAPE_PROXYTYPE: // RENDER SPHERE ====================
				App->physics->RenderSphereCollider(colliderComponent->physBody);
				break;
			case BroadphaseNativeTypes::CAPSULE_SHAPE_PROXYTYPE: // RENDER CAPSULE ==================
				App->physics->RenderCapsuleCollider(colliderComponent->physBody);
				break;
			case BroadphaseNativeTypes::CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE: // RENDER MESH =========
				App->physics->RenderMeshCollider(colliderComponent->physBody);
				break;
			} 

			glColor3f(255.0f, 255.0f, 255.0f);
			glLineWidth(1.f);
		}
	}

	if (App->editor->gl_Lighting) glEnable(GL_LIGHTING);
}

bool ModuleRenderer3D::IsInsideFrustum(const CCamera* camera, const AABB& aabb)
{
	// Check if frustum culling is enabled for the camera.
	if (camera->enableFrustumCulling) {

		// Get the planes of the camera frustum.
		Plane frustumPlanes[6];
		camera->frustum.GetPlanes(frustumPlanes);

		// Get the corner points of the AABB.
		float3 cornerPoints[8];
		aabb.GetCornerPoints(cornerPoints);

		// Loop through each plane of the frustum.
		for (int i = 0; i < 6; ++i) {

			// Counter for points inside the frustum plane.
			uint pointsInside = 8;

			// Loop through each corner point of the AABB.
			for (int j = 0; j < 8; ++j)
			{
				// Check if the corner point is on the positive side of the frustum plane.
				if (frustumPlanes[i].IsOnPositiveSide(cornerPoints[j]))
				{
					// Decrement the counter if the point is on the positive side.
					--pointsInside;
				}
			}

			// If all corner points are outside the frustum plane, the AABB is outside the frustum.
			if (pointsInside == 0)
			{
				return false;
			}

		}

	}

	// If frustum culling is not enabled or the AABB is inside the frustum, return true.
	return true;
}

void ModuleRenderer3D::GetUIGOs(GameObject* go, std::vector<C_UI*>& listgo)
{
	if (go->GetComponent(ComponentType::UI) != nullptr)
	{
		C_UI* ui = static_cast<C_UI*>(go->GetComponent(ComponentType::UI));
		listgo.push_back(ui);
	}

	if (!go->mChildren.empty())
	{
		for (auto i = 0; i < go->mChildren.size(); i++)
		{
			GetUIGOs(go->mChildren[i], listgo);
		}
	}
}

void ModuleRenderer3D::DrawUIElements(bool isGame, bool isBuild)
{
	if (isGame)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		isBuild ? glOrtho(0.0, External->window->width, External->window->height, 0.0, 1.0, -1.0) : glOrtho(0.0, App->editor->gameViewSize.x, App->editor->gameViewSize.y, 0.0, 1.0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
	}

	//Get UI elements to draw
	std::vector<C_UI*> listUI;

	for (auto it = App->scene->vCanvas.begin(); it != App->scene->vCanvas.end(); ++it)
	{
		GetUIGOs(*it, listUI);

	}

	if (!listUI.empty())
	{
		for (auto i = listUI.size() - 1; i > 0; --i)
		{
			if (listUI[i]->mOwner->active && listUI[i]->active)
			{
				listUI[i]->Draw(isGame);
				//listUI[i]->DebugDraw();

			}
		}
	}

}

void ModuleRenderer3D::DrawLightsDebug()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (App->lightManager->IsLightDebugEnabled()) {

		for (auto& it = App->lightManager->lights.begin(); it != App->lightManager->lights.end(); ++it) {

			if ((*it)->debug && (*it)->lightGO->active && (*it)->lightGO->GetComponent(ComponentType::LIGHT)->active) {

				(*it)->Render();

			}	

		}

	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::DrawGameObjects()
{
	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		CTransform* transformComponent = (CTransform*)(*it)->GetComponent(ComponentType::TRANSFORM);
		CMesh* meshComponent = (CMesh*)(*it)->GetComponent(ComponentType::MESH);
		CMaterial* materialComponent = (CMaterial*)(*it)->GetComponent(ComponentType::MATERIAL);
		CAnimation* animationComponent = (CAnimation*)(*it)->GetComponent(ComponentType::ANIMATION);

		if (animationComponent != nullptr && animationComponent->active) {
			for (int i = 0; i < (*it)->mChildren.size(); i++) {
				if ((*it)->mChildren[i]->GetComponent(MATERIAL)) {
					CMaterial* cmat = (CMaterial*)(*it)->mChildren[i]->GetComponent(MATERIAL);
					CTransform* ctrans = (CTransform*)(*it)->mChildren[i]->GetComponent(TRANSFORM);
					cmat->shader.UseShader(true);
					std::vector<float4x4> transforms = animationComponent->animator->GetFinalBoneMatrices();
					for (int i = 0; i < transforms.size(); i++) {
						cmat->shader.SetMatrix4x4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
						cmat->shader.SetShaderUniforms(&ctrans->mGlobalMatrix, (*it)->selected);
					}
				}
			}
		}

		if ((*it)->active && meshComponent != nullptr && meshComponent->active)
		{
			if (IsInsideFrustum(External->scene->gameCameraComponent, meshComponent->rMeshReference->globalAABB))
			{

				if (materialComponent != nullptr && materialComponent->active) {

					for (auto& textures : materialComponent->rTextures) {

						textures->BindTexture(true);

					}

					materialComponent->shader.UseShader(true);

					if (animationComponent != nullptr && animationComponent->active) {
						std::vector<float4x4> transforms = animationComponent->animator->GetFinalBoneMatrices();
						for (int i = 0; i < transforms.size(); i++) {
							materialComponent->shader.SetMatrix4x4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
						}
					}
					materialComponent->shader.SetShaderUniforms(&transformComponent->mGlobalMatrix, (*it)->selected);
				}

				meshComponent->rMeshReference->Render();

				if (materialComponent != nullptr && materialComponent->active) {

					materialComponent->shader.UseShader(false);

					for (auto& textures : materialComponent->rTextures) {

						textures->BindTexture(false);

					}

				}

			}

		}

	}

}

void ModuleRenderer3D::ClearModels()
{
	models.clear();
}

void ModuleRenderer3D::EnableAssimpDebugger()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void ModuleRenderer3D::CleanUpAssimpDebugger()
{
	aiDetachAllLogStreams();
}
