#pragma once

#include "Globals.h"
#include "Timer.h"

#include<vector>
#include<string>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleEditor;
class ModuleCamera3D;
class ModuleTexture;
class ModuleMesh;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleTexture* texture;
	ModuleMesh* mesh;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float FPS();
	float DT();
	float MS();
	std::vector<float> mFPSLog;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};