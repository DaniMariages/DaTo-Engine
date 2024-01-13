#pragma once
#include <string>
#include <vector>

#include "ComponentTransform.h"
#include "Component.h"
#include "ComponentUI.h"
#include "Random.h"

class Component;
enum class typeComponent;

class GameObject
{
public:
	std::string Name;
	std::vector<Component*> components;
	GameObject* Parent;
	std::vector<GameObject*> children;

	//ID of each Game Object
	int ID;

	//Bool to know if game object is selectable with MousePicking
	bool selectableWithMP = true;

	bool selected = false;

	bool active = true;
	bool drawTexture = true;

	Random random;

public:
	GameObject(std::string Name, GameObject* parent);
	~GameObject();

	void Enable(); //Start up + bool toggle
	void Disable();//Clean up + bool toggle

	//Enable and disable texture of a Game Object
	void EnableTexture();
	void DisableTexture();

	//Enable and disable the textures of parent and childrens
	void EnableTextureParent();
	void DisableTextureParent();

	//Enable and disable parent Game Object, including their children
	void EnableParent(); 
	void DisableParent();

	void SetId();

	//Function to change the name of a game object
	void ChangeName(const char* name);

	void Update();

	void SetParent(GameObject* parent);
	Component* AddComponent(Component* component);
	Component* GetComponent(typeComponent type);
	bool HasComponent(typeComponent type);
	Component* CreateComponent(typeComponent comp_type, bool active = true, uint width = 0, uint height = 0);
	ComponentUI* CreateComponentUI(UI_type comp_type, uint width, uint height, ComponentCanvas* canvas, const char* str, uint x = 0, uint y = 0, bool active = true);

	std::vector<Component*> GetComponents(typeComponent type);
	GameObject* AddChildren(GameObject* children);
	void EraseChild(GameObject* child);

	ComponentTransform* transform = nullptr;

private:

	Random	id;
};
