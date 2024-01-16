#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "../External/ImGui/imgui.h"
#include "../External/ImGui/backends/imgui_impl_opengl3.h"
#include "../External/ImGui/backends/imgui_impl_sdl2.h"
#include "../External/ImGuizmo/ImGuizmo.h"

#include <vector>
#include <string>

#define YELLOW ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
class GameObject;

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void Config();
	void MainMenuBar();
	void DemoWindow();

	void DrawInspector();
	void DrawHierarchy();
	void Inspector(GameObject* gameObject);
	void HierarchyWindow(GameObject* gameObject);
	void DrawSceneAlert();

	// Mouse picking
	void MousePicking(ImVec2 mousePosition, ImVec2 windowPos, ImVec2 windowSize, float frameHeightOffset);
	ImVec2 NormalizePoint(float x, float y, float w, float h, ImVec2 originalPoint);

	void RequestBrowser(const char* path);
	void UpdateFPS(const float aFPS);
	void ModuleEditor::Console();
	void DrawPausePlay();

public: 

	GameObject* childObject = nullptr;
	std::vector<std::string> log_history;

	float aFPS;
	char label[32];

	bool show_console_window = true;
	bool show_demo_window = false;
	bool show_metrics_window = false;
	bool show_config_window = true;
	bool show_hierarchy_window = true;
	bool show_inspector_window = true;
	bool show_console = true;
	bool show_delete_scene_modal = false;

	//This bool helps to dont close the PopUp just when the other bool is pressed
	bool dontShowAgainPressed = false;

	//Bool to show or not show the Atention PopUp
	bool dontShowMeAgain = false;

	//Bool of all game objects options
	bool drawAll = true;
	bool drawAllFaces = false;
	bool drawAllVertex = false;
	bool drawTextures = true;
	bool drawAllBoxes = true;

	//Bool of selected game object options
	bool drawSelected = true;
	bool drawSelectedVertex = false;
	bool drawSelectedFaces = false;
	bool drawSelectedTexture = true;
	bool drawSelectedBoxes = true;

	bool shader = true;
	bool cullface = true;
	bool Wireframe = false;
	bool lights = true;

	bool fullscreen = false;
	bool borderless = false;

	bool IoConfirmer = false;

	ImVec2 GameWindowSize;
	ImVec2 GameWindowPos;
	ImVec2 mousePosInViewport;

};

#endif //MODULE_EDITOR