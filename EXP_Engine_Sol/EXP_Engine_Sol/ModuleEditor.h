#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>

#define YELLOW ImVec4(1.0f, 1.0f, 0.0f, 1.0f)

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool CleanUp() override;

	void Config();

	SDL_GLContext context;
	/*void AddFPS(const float aFPS);*/

	std::vector<std::string> log_record;
	float aFPS;

	void UpdateFPS(const float aFPS);

	bool show_console_window = true;

	bool show_demo_window = false;
	bool show_metrics_window = false;
	bool Draw = true;
	bool Normals = false;
	bool Wireframe = false;
	bool vertex = false;

	bool fullscreen = false;
	bool borderless = false;

};

#endif //MODULE_EDITOR