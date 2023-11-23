#include "ModuleScene.h"
#include "Globals.h"
#include "ModuleCamera3D.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rootGameObject = CreateGameObject("Scene", nullptr);
	editorCamera = CreateGameObject("Editor camera", rootGameObject);
}

// Destructor
ModuleScene::~ModuleScene()
{}

// Called before render is available
bool ModuleScene::Init()
{
	LOG("Creating Module Scene");
	bool ret = true;

	editorCamera->AddComponent(App->camera->editorCamera);

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

void ModuleScene::DeleteGameObject(GameObject* gameObject)
{
	LOG("Deleting Game Object: %s.", gameObject->Name.c_str());

	//If the game object has children delete them first
	if (!gameObject->children.empty())
	{
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
		{
			for (unsigned int j = 0; j < gameObject->children.size(); ++j)
			{
				//Delete the childrens from game objects list
				if (gameObjects[i] == gameObject->children[j])
					gameObjects.erase(gameObjects.begin() + i);
			}
		}

		//Delete all the children
		do
		{
			int i = 0;
			gameObject->children.erase(gameObject->children.begin() + i);
			i++;

		} while (!gameObject->children.empty());
	}

	//Delete of children parent list
	for (unsigned int i = 0; i < gameObject->Parent->children.size(); ++i)
	{
		if (gameObject == gameObject->Parent->children[i])
			gameObject->Parent->children.erase(gameObject->Parent->children.begin() + i);
	}
	
	//When references deleted, delete the game Object
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObject == gameObjects[i]) 
			gameObjects.erase(gameObjects.begin() + i);
	}

	//Set the gameObjects null
	gameObject = nullptr;
	gameObjectSelected = nullptr;
}
