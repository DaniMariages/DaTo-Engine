#pragma once

#include "Component.h"
#include "ComponentTransform.h"
#include "Color.h"

class GameObject;

class ComponentCanvas : public Component
{
public:
	GameObject* parent;

	float widthPanel;
	float heigthPanel;

	uint posX;
	uint posY;

	ComponentTransform* comp_transform;

public:
	ComponentCanvas(GameObject* gameObject, float width, float heigt, uint PosX, uint PosY);
	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	void Enable();
	void Update();
	void Disable();
	void Draw();

	void ShowInfo();

private:

};