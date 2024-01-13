#pragma once
#include "Component.h"
#include "Globals.h"

class GameObject;

class ComponentCanvas : public Component {
public:
	ComponentCanvas(typeComponent cType, bool act, GameObject* objs, uint w, uint h);
	ComponentCanvas(GameObject* parent);
	~ComponentCanvas() {};

	void Update();

	void DebugDraw();

public:
	uint width = 0, height = 0;
};
