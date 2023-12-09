#include "Application.h"
#include "Globals.h"

#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "../External/MathGeoLib/include/Math/Quat.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	editorCamera = new ComponentCamera(nullptr);

	editorCamera->SetPos(0.0f, 2.0f, 8.0f);
	editorCamera->LookAt(float3(0.f, 0.f, 0.f));
	editorCamera->SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	float3 newPos(0, 0, 0);
	float speed = 3.0f * dt;

	//Increase the camera speed while left shift is pressed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed *= 2;

	//Camera Zoom (Mouse Wheel)
	editorCamera->Zoom(newPos, speed * 20); //Speed is to low for Zoom

	//Focus camera without moving it (on 0,0,0)
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->scene->gameObjectSelected != nullptr)
		{
			editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
		}
		else editorCamera->LookAt(newPos);
		
		//Rotate around (0,0,0) if Left Alt and Left Click are pressed 
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (App->scene->gameObjectSelected != nullptr)
			{
				editorCamera->Orbit(App->scene->gameObjectSelected->transform->GetPosition(), speed, dt);
			}
			else editorCamera->Orbit(newPos, speed, dt);
		}
	}

	//Focus Camera moving it (to Distance)
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if (App->scene->gameObjectSelected != nullptr)
		{
			editorCamera->Focus(App->scene->gameObjectSelected->transform->GetPosition(), 6.0f);
		}
		else editorCamera->Focus(newPos, 6.0f);
	}

	//Rotate the camera (Right click pressed)
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		editorCamera->Rotate(dt);

		//Move the camera with WASD if right click is pressed
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			editorCamera->Move(newPos, speed);
		}
	}
	
	//Pan (Mouse wheel pressed)
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) 
	{
		editorCamera->Pan(newPos, speed, dt);
	}

	//Top view
	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
		TopView();

	//Front view
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		FrontView();

	//Back view
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		BackView();

	//Side view (left)
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Side_1View();

	//Side view (right)
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Side_2View();

	editorCamera->UpdatePos(newPos);
	return UPDATE_CONTINUE;
}

void ModuleCamera3D::MousePickingRay(float posX, float posY)
{
	mousePickingRay = editorCamera->frustum.UnProjectLineSegment(posX, posY);
	App->scene->SelectGameObject(mousePickingRay);
}

void ModuleCamera3D::TopView()
{
	float3 newPos(0, 0, 0);

	if (App->scene->gameObjectSelected != nullptr)
	{
		editorCamera->SetPos(App->scene->gameObjectSelected->transform->GetPosition() + float3(0.0f, 10.0f, 0.0f));
		editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
	}
	else
	{
		newPos = float3(0.0f, 10.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}
}

void ModuleCamera3D::FrontView()
{
	float3 newPos(0, 0, 0);

	if (App->scene->gameObjectSelected != nullptr)
	{
		editorCamera->SetPos(App->scene->gameObjectSelected->transform->GetPosition() + float3(0.0f, 0.0f, -10.0f));
		editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
	}
	else
	{
		newPos = float3(0.0f, 0.0f, -10.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}
}

void ModuleCamera3D::Side_1View()
{
	float3 newPos(0, 0, 0);

	if (App->scene->gameObjectSelected != nullptr)
	{
		editorCamera->SetPos(App->scene->gameObjectSelected->transform->GetPosition() + float3(10.0f, 00.0f, 0.0f));
		editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
	}
	else
	{
		newPos = float3(10.0f, 00.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}
}

void ModuleCamera3D::Side_2View()
{
	float3 newPos(0, 0, 0);

	if (App->scene->gameObjectSelected != nullptr)
	{
		editorCamera->SetPos(App->scene->gameObjectSelected->transform->GetPosition() + float3(-10.0f, 00.0f, 0.0f));
		editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
	}
	else
	{
		newPos = float3(-10.0f, 00.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}
}

void ModuleCamera3D::BackView()
{
	float3 newPos(0, 0, 0);

	if (App->scene->gameObjectSelected != nullptr)
	{
		editorCamera->SetPos(App->scene->gameObjectSelected->transform->GetPosition() + float3(0.0f, 0.0f, 10.0f));
		editorCamera->LookAt(App->scene->gameObjectSelected->transform->GetPosition());
	}
	else
	{
		newPos = float3(0.0f, 0.0f, 10.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}
}