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


	SDL_GLContext context;
	/*void AddFPS(const float aFPS);*/

	std::vector<float> mFPSLog;
	float aFPS;

	void UpdateFPS(const float aFPS);
};

#endif //MODULE_EDITOR