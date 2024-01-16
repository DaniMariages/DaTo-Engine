#pragma once
#include "Component.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

enum functions
{
	PASS_SCENE,
	DEFAULT,
	MAX
};

struct actions
{
	vector<string> nameOfFunctions = { "Pass scene", "None" };
};

class ComponentButton : public ComponentUI
{
public:
	GameObject* parent;

	uint widthPanel;
	uint heigthPanel;

	bool isPressed;

	functions actualFunction;

public:
	ComponentButton(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentButton();

	bool OnClick(int* action);

	bool OnIdle(ComponentUI* UI_Element);
	bool OnHover(ComponentUI* UI_Element);
	bool OnClicked();

	void ShowInfo(int* action);

	void PassScene();

	void CreatePauseMenu();
};