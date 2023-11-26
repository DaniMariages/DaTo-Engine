#pragma once
class GameObject;

enum class typeComponent {
	Error = -1,
	Transform,
	Mesh,
	Material,
	Camera,
};

class Component {
public:
	Component(GameObject* parent) : active(true), parent(parent), type(typeComponent::Error) {};

	typeComponent type;
	bool active;
	GameObject* parent;

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void DrawInspector() {};
};