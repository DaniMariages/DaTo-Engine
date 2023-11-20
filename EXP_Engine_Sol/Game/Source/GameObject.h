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

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle

	void Update();

	void SetParent(GameObject* parent);
	Component* AddComponent(Component* component);
	Component* GetComponent(typeComponent type);

	std::vector<Component*> GetComponents(typeComponent type);
	GameObject* AddChildren(GameObject* children);

	bool selected = false;
};
