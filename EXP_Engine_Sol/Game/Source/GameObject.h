#pragma once
#include <string>
#include <vector>

class Component;
enum class typeComponent;

class GameObject
{
public:
	std::string Name;
	std::vector<Component*> components;
	bool active;

	GameObject* Parent;
	std::vector<GameObject*> children;

	GameObject(std::string Name, GameObject* parent);
	~GameObject();

	void Enable(); //Start up + bool toggle
	void Disable();//Clean up + bool toggle

	//Enable parent Game Object, including their children
	void EnableParent(); 

	//Disable parent Game Object, including their children
	void DisableParent();

	void ChangeName(const char* name);

	void Update();

	void SetParent(GameObject* parent);
	Component* AddComponent(Component* component);
	Component* GetComponent(typeComponent type);

	std::vector<Component*> GetComponents(typeComponent type);
	GameObject* AddChildren(GameObject* children);

	bool selected = false;
};
