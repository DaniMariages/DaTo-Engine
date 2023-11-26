#pragma once
#include "../External/MathGeoLib/include/Math/float4x4.h"

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

	virtual void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity) {};
};