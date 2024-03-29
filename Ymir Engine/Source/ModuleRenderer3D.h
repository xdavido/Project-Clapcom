#pragma once

#include "External/Glew/include/glew.h"
#include "External/SDL/include/SDL_opengl.h"
#include <gl/GL.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Source/External/Glew/libx86/glew32.lib")

#include "External/DevIL/include/il.h"	
#include "External/DevIL/include/ilu.h"	

#include "External/MathGeoLib/include/Math/float3x3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "External/MathGeoLib/include/Geometry/Plane.h"

#include "External/MathGeoLib/include/Geometry/AABB.h"
#include "External/MathGeoLib/include/Geometry/OBB.h"

#include "External/FreeType/include/ft2build.h"
#include "External/FreeType/include/freetype/freetype.h"
#include "External/FreeType/include/freetype/ftglyph.h"
#pragma comment(lib, "Source/External/FreeType/libx86/freetype.lib")


#ifdef _DEBUG
#pragma comment (lib, "Source/External/MathGeoLib/libx86/lib_Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "Source/External/MathGeoLib/libx86/lib_Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG

#include <fstream>
#include <vector>
#include "Module.h"
#include "Globals.h"
#include "GL_Light.h"
#include "Primitive.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "UI_Text.h"

#define MAX_GL_LIGHTS 8

class GameObject;
class CCamera;
class ParticleEmitter;

class ModuleRenderer3D : public Module
{
public:

	ModuleRenderer3D(Application* app, bool start_enabled = true);
	virtual ~ModuleRenderer3D();

	bool Init() override;
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void OnResize(int width, int height);
	void SetGameCamera(CCamera* cam = nullptr);

	void DrawGameObjects();
	void ClearModels();

	void EnableAssimpDebugger();
	void CleanUpAssimpDebugger();

	void HandleDragAndDrop();
	bool IsFileExtension(const char* directory, const char* extension);

	void ApplyCheckerTexture();
	void ClearActualTexture();

	void ReloadTextures();

	void DrawBox(float3* vertices, float3 color);

	// Draw Meshes Bounding Boxes
	void DrawBoundingBoxes();

	// Handle Frustum Culling
	bool IsInsideFrustum(const CCamera* camera, const AABB& aabb);

	// Draw Physics Colliders
	void DrawPhysicsColliders();
	// Draw UI
	void GetUIGOs(GameObject* go, std::vector<C_UI*>& listgo);
	void DrawUIElements(bool isGame, bool isBuild);

	void DrawLightsDebug();

	void DrawParticles();

public:

	GL_Light gl_lights[MAX_GL_LIGHTS];
	SDL_GLContext context;

	CPlane Grid;
	bool showGrid = true;

	// 3D Models
	std::vector<Model> models;

	// UI Models
	std::vector<C_UI*> listUI; // clean up 

	bool texturingEnabled = true;

	//font
	Font* defaultFont;

	std::vector<ParticleEmitter*> particleEmitters;
	bool initParticles = false;

};