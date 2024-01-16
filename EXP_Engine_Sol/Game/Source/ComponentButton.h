#pragma once
#include "Component.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

class ComponentButton : public ComponentUI
{
public:
	GameObject* parent;

	uint widthPanel;
	uint heigthPanel;

	bool isPressed;

public:
	ComponentButton(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentButton();

	bool OnClick();

	bool OnIdle(ComponentUI* UI_Element);
	bool OnHover(ComponentUI* UI_Element);
	bool OnClicked();

	void ShowInfo(int* action);

	void PassScene();

	void CreatePauseMenu();
};