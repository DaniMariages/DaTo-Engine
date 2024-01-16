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

	unsigned int posX;
	unsigned int posY;

	ComponentTransform* comp_transform;

public:
	ComponentCanvas(GameObject* gameObject, float width, float heigt, unsigned int PosX, unsigned int PosY);
	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	void Enable();
	void Update();
	void Disable();
	void Draw();

	void ShowInfo();

private:

};