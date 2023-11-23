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

	float speed = 20.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed *= 2;

	// Mouse wheel Zoom In and Zoom Out handling

	editorCamera->Zoom(newPos, speed);

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) 
	{
		editorCamera->Pan(newPos, speed, dt);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) 
	{
		editorCamera->Move(newPos, speed);
		editorCamera->Rotate(speed, dt);
	}

	editorCamera->UpdatePos(newPos);

	if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) 
	{
		editorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));
	}

	return UPDATE_CONTINUE;
}