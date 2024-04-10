#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include "Log.h"
#include "Primitive.h"
#include <vector>
#include <string>
#include <filesystem>

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include "External/ImGui/misc/cpp/imgui_stdlib.h"
#include "External/ImGui/imgui_internal.h"

#include "External/ImGuizmo/include/ImGuizmo.h"

#include "NodeEditor.h"
#include "ShaderEditor.h"

#include "Texture.h"
#include "ScriptEditor.h"

class GameObject;

class ModuleEditor : public Module
{
public:

	// Constructor
	ModuleEditor(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleEditor();

	// Module functions
	bool Init() override;
	update_status Update(float dt) override;
	bool CleanUp() override;

	// Draws the editor on the window
	void DrawEditor();
	void PrimitivesMenu();
	void CreateCameraMenu();
	void UIMenu();
	void LightsMenu();

	void SaveAs();
	void SaveAsPrefabPopUp(GameObject* prefab);
	void RenderSaveAsPrefabPopUp();

	// Manages the docking functionality with the main window
	void WindowDockSpaceManagement();

	// OpenGL settings checkbox toggle encapsulation
	void Toggle_GL_DepthTesting(bool depthTesting);
	void Toggle_GL_FaceCulling(bool faceCulling);
	void Toggle_GL_Lighting(bool lighting);
	void Toggle_GL_ColorMaterial(bool colorMaterial);
	void Toggle_GL_TextureMapping2D(bool textureMapping2D);
	void Toggle_GL_TextureMapping3D(bool textureMapping3D);
	void Toggle_GL_Blending(bool blending);
	void Toggle_GL_MSAA(bool msaa);
	void Toggle_GL_StencilTesting(bool stencilTesting);
	void Toggle_GL_ScissorTesting(bool scissorTesting);
	void Toggle_GL_AlphaTesting(bool alphaTesting);
	void Toggle_GL_PointSprites(bool pointSprites);
	void Toggle_GL_Fog(bool fog);
	void Toggle_GL_PointSmooth(bool pointSmooth);
	void Toggle_GL_LineSmooth(bool lineSmooth);
	void Toggle_GL_Normalization(bool normalization);
	void Toggle_GL_PolygonOffset(bool polygonOffset);
	void Toggle_GL_WireframeMode(bool wireframe);

	// Functions related to Framerate Histograms
	void AddFPS(const float aFPS);
	void AddDT(const float aDT);
	void AddMS(const float aMS);

	// Function to encapsulate website redirection OnClick()
	void RequestBrowser(const char* url);

	// Window settings checkbox toggle encapsulation
	void ToggleFullscreen(bool fullscreen);
	void ToggleResizable(bool resizable);
	void ToggleBorderless(bool borderless);
	void ToggleFullscreenDesktop(bool fullscreenDesktop);

	// Renderer3D settings checkbox toggle encapsulation
	void ToggleVSync(bool vsync);

	// Editor settings checkbox toggle encapsulation
	void ToggleLightMode(bool lightMode);

	// Meshes normal settings checkbox toggle encapsulation
	void ToggleMeshesVertexNormals(bool showVertexNormals);
	void ToggleMeshesFaceNormals(bool showFaceNormals);

	// Hardware Detection
	void ShowPlatformInfo();
	void ShowCPUInfo();
	void ShowGPUInfo();
	void ShowRAMInfo();
	void ShowDiskInfo();

	// Function to encapsulate About modal window
	void AboutModalWindowContent();

	// Function to encapsulate Reading files
	std::string ReadFile(const std::string& filename);

	// Function to show log output on the editor
	void RedirectLogOutput();

	// Function to show memleaks file output on the editor
	void MemoryLeaksOutput();

	// Function to show memleaks file output on the editor
	void AssimpLogOutput();

	// Function to encapsulate GameObjects hierarchy
	void DrawHierarchy();
	void CreateHierarchyTree(GameObject* node);
	//void DestroyHierarchyTree(GameObject* node);

	// Function to encapsulate GameObjects inspector
	void DrawInspector();

	// Function to manage Gizmo
	void DrawGizmo(const ImVec2& sceneWindowPos, const ImVec2& sceneContentRegionMax, const float& sceneFrameHeightOffset);

	// Project assets window
	
	// Function to draw File Explorer
	void DrawFileExplorer(const std::string& rootFolder);

	// Function to draw Assets Window
	void CreateNewFolder();
	void DrawAssetsWindow(const std::string& assetsFolder);

	// Function to draw Delete Asset Popup
	void DeleteAssetConfirmationPopup(const char* filePath);
	void RenderDeleteAssetConfirmationPopup();

	// Function to draw Library Window
	void DrawLibraryWindow(const std::string& libraryFolder);

	// Function to delete a file
	void DeleteFileAndRefs(const char* filePath);

	// Function to handle Mouse Picking
	void MousePickingManagement(const ImVec2& mousePosition, const ImVec2& sceneWindowPos, const ImVec2& sceneWindowSize, const float& sceneFrameHeightOffset);

	// Support function to normalize the given coordinates based on the specified ImGui Window Bounding Box.
	ImVec2 NormalizePoint(const float& x, const float& y, const float& w, const float& h, const ImVec2& originalPoint);

public:
	update_status exit;

	// Enables ImGui to render additional data
	bool UpdateAndRenderAdditionalPlatformWindows = false;

	// Booleans to Open/Close Editor windows
	
	bool showApplication = true;
	bool showConsole = true;
	bool showMemoryLeaks = false;
	bool showAssimpLog = false;
	bool showHierarchy = true;
	bool showInspector = true;

	bool showScene = true;
	bool showGame = true;
	bool showSaveAs = false;
	bool showResources = true;
	bool showFileExplorer = true;
	bool showAssets = true;
	bool showLibrary = true;

	bool showNodeEditor = false;

	bool showScriptingEditor = true;
	bool showShaderEditor = false;

	// OpenGL settings checkbox toggle booleans
	bool gl_DepthTesting = true;
	bool gl_FaceCulling = true;
	bool gl_Lighting = true;
	bool gl_ColorMaterial = true;
	bool gl_TextureMapping2D = true;
	bool gl_TextureMapping3D = false;
	bool gl_Blending = false;
	bool gl_MSAA = false;
	bool gl_StencilTesting = false;
	bool gl_ScissorTesting = false;
	bool gl_AlphaTesting = false;
	bool gl_PointSprites = false;
	bool gl_Fog = false;
	bool gl_PointSmooth = false;
	bool gl_LineSmooth = false;
	bool gl_Normalization = false;
	bool gl_PolygonOffset = false;
	bool gl_WireframeMode = false;

	// Vectors related to Framerate Histograms
	std::vector<float> FPSvec;
	std::vector<float> DTvec;
	std::vector<float> MSvec;

	// Window settings checkbox toggle booleans
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullscreenDesktop = false;

	// Renderer3D settings checkbox toggle booleans
	bool vsync = true;

	// Editor settings checkbox toggle booleans
	bool lightMode = false;
	bool showImGuiDemo = false;

	// Window sliders
	int windowWidth = 0;
	int windowHeight = 0;
	float opacity = 0.0f;

	// About modal window inside help menu
	bool showAboutPopUp = false;
	std::string licenseFileContents;

	// Script select name modal window
	bool showNewScriptPopUp = false;

	// Memory Leaks file
	std::string memleaksFileContents;

	// Assimp Log file
	std::string AssimpLogFileContents;

	// 3D Meshes Configuration
	bool showVertexNormals = false;
	bool showFaceNormals = false;

	// File Contents Modal Window handlers
	std::string selectedFilePath;
	bool showModal = false;

	// Drag and Drop Hierarchy handlers
	GameObject* hoveredGO = nullptr;
	GameObject* draggedGO = nullptr;

	// ImGuizmo handlers
	ImGuizmo::OPERATION gizmoOperation;
	ImGuizmo::MODE gizmoMode;
	float snapValue;// Snap Guizmo
	float4x4 modelMatrix;

	// Assets folder
	std::string currentDir;
	std::string selectedDir;

	std::string currentFile;
	std::string selectedFile;
	bool rmbMenu = false;
	bool createFolder = false;

	// Node Editor
	NodeEditorWindow nodeEditor;

	// Shader Editor
	ShaderEditor shaderEditor;

	// Editor Icons
	Texture folderIcon;
	Texture fileIcon;
	Texture imageIcon;
	Texture modelIcon;
	Texture shaderIcon;
	Texture sceneIcon;
	Texture prefabIcon;
	Texture animIcon;

	//New Component
	int newComponent = 0; // 0 = default, 1 = RigidBody, 2 = Collider

	//Script Editor
	ScriptEditor* scriptEditor;

	// Stuff needed for UI, don't know if it is 100% needed
	ImVec2 gameViewPos;
	ImVec2 gameViewSize;
	float2 mouse;
	ImGuiContext* g;

	//Name Script
	std::string script_name;

	//Ignore Meta Files
	bool shouldIgnoreMeta = true;
};

#endif // MODULE_EDITOR