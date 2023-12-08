#pragma once
#include <string>
#include <vector>

#include "ComponentTransform.h"
#include "Random.h"

class Component;
enum class typeComponent;

class GameObject
{
public:
	std::string Name;
	std::vector<Component*> components;
	bool active;
	bool drawTexture;

	GameObject* Parent;
	std::vector<GameObject*> children;

	uint UID;
	Random randomInt;

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

	//Get and Set the Unique ID of a Game Object
	const uint GetUID() const;
	void SetUID(uint UID);

	//Function to know if a Game Object is the Root Game Object
	bool IsRootObject();

	//Function to know if a Game Object is selected
	bool IsSelected();

	//Function to change the name of a game object
	void ChangeName(const char* name);

	void Update();

	void SetParent(GameObject* parent);
	Component* AddComponent(Component* component);
	Component* GetComponent(typeComponent type);
	bool HasComponent(typeComponent type);

	std::vector<Component*> GetComponents(typeComponent type);
	GameObject* AddChildren(GameObject* children);
	void EraseChild(GameObject* child);

	ComponentTransform* transform = nullptr;

	bool selected = false;
};
