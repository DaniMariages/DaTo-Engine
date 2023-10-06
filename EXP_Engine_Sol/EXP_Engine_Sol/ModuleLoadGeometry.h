#pragma once

#include "Assimp/include/cimport.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/scene.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ModuleLoadGeometry : public Module
{
public:
	ModuleLoadGeometry(Application* app, bool start_enabled = true);
	~ModuleLoadGeometry();


};