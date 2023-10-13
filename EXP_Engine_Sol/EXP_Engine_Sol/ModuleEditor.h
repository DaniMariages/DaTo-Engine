#pragma once
#ifndef MODULE_EDITOR
#define MODUEL_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>

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

	bool Draw = true;
	bool Normals = false;
	bool Wireframe = false;
	bool vertex = false;

};

#endif //MODULE_EDITOR