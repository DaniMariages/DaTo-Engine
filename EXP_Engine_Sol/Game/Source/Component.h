#pragma once
#include <string>

class GameObject;

enum class typeComponent {
	Error = -1,
	Transform,
	Mesh,
	Material,
};

class Component {
	//Properties
public:
	typeComponent type;
	bool active;
	GameObject* parent;

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void DrawInspector() {};
	Component(GameObject* parent) : active(true), parent(parent), type(typeComponent::Error) {};

};