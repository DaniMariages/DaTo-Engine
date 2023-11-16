#pragma once
#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"
#include "../External/Glew/include/glew.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:

	
};