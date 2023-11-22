#include "Globals.h"
#include "Application.h"

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

	editorCamera->SetPos(8.0f, 2.0f, 8.0f);
	editorCamera->SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	editorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));

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
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 3.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	
	if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	editorCamera->Zoom(newPos, speed);

	//Focus camera
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) 
	{
		editorCamera->LookAt(newPos);
	}
	
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		editorCamera->Move(newPos, speed);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT )
	{
		editorCamera->LookAt(newPos);

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			editorCamera->Rotate(speed, dt);
		}
	}

	//Top view
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->editor->top == true) 
	{
		newPos = float3(0.0f, 10.0f, 0.0f);
		editorCamera->LookAt(newPos);
	}

	//Front view
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->editor->front == true) 
	{
		newPos = float3(0.0f, 0.0f, 10.0f);
		editorCamera->LookAt(newPos);
	}

	//Side view
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->editor->side_1 == true) 
	{
		newPos = float3(10.0f, 00.0f, 0.0f);
		editorCamera->LookAt(newPos);
	}
	

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->editor->side_2 == true) 
	{
		newPos = float3(-10.0f, 00.0f, 0.0f);
		editorCamera->LookAt(newPos);
	}


	return UPDATE_CONTINUE;
}