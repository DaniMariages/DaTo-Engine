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

	//Views of game object
	void TopView();
	void FrontView();
	void Side_1View();
	void Side_2View();
	void BackView();

	void MousePickingRay(float posX, float posY);

public:
	
	LineSegment mousePickingRay;
	ComponentCamera* editorCamera;
};