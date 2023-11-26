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

	//Move the camera Up (R) or Down (F)
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	//Focus camera without moving it (on 0,0,0)
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		editorCamera->LookAt(newPos);

		//Rotate around (0,0,0) if Left Alt and Left Click are pressed 
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			editorCamera->Orbit(newPos, speed, dt);
		}
	}

	//Focus Camera moving it (to Distance)
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		editorCamera->Focus(newPos, 6.0f);

		if (App->scene->gameObjectSelected)
		{
			editorCamera->Focus(App->scene->gameObjectSelected->transform->GetPosition(), 6.0f);
		}
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
	
	//PAN (Mouse wheel pressed)
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) 
	{
		editorCamera->Pan(newPos, speed, dt);
	}

	//Top view
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->editor->top == true)
	{
		newPos = float3(0.0f, 10.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}

	//Front view
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->editor->front == true)
	{
		newPos = float3(0.0f, 0.0f, 10.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));

	}

	//Side view (left)
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->editor->side_1 == true)
	{
		newPos = float3(10.0f, 00.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}

	//Side view (right)
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->editor->side_2 == true)
	{
		newPos = float3(-10.0f, 00.0f, 0.0f);
		editorCamera->SetPos(newPos);
		editorCamera->LookAt(float3(0, 0, 0));
	}

	editorCamera->UpdatePos(newPos);

	return UPDATE_CONTINUE;
}