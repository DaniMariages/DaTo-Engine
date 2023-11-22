#pragma once
#include "Module.h"
#include "Globals.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"

#include "ComponentCamera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	
	ComponentCamera* editorCamera;

};