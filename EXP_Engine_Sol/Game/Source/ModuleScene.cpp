#include "Globals.h"
#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rootGameObject = CreateGameObject("Scene", nullptr);
}

// Destructor
ModuleScene::~ModuleScene()
{}

// Called before render is available
bool ModuleScene::Init()
{
	LOG("Creating Module Scene");
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleScene::CleanUp()
{
	LOG("Destroying Module Scene");


	return true;
}

GameObject* ModuleScene::CreateGameObject(std::string Name, GameObject* parent)
{
	GameObject* newGameObject = new GameObject(Name, parent);

	if (parent != nullptr) parent->AddChildren(newGameObject);

	gameObjects.push_back(newGameObject);

	return newGameObject;
}
