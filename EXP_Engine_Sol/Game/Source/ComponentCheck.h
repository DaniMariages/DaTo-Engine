#pragma once
#pragma once
#ifndef CHECKERUI_H_
#define CHECKERUI_H_

#include "Component.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

enum _functions
{
	_VSYNC,
	_DRAG,
	_DEF,
	_MAX
};

struct values
{
	vector<string> nameOfFunctions = { "Vysnc" , "Drag", "None" };
};

class ComponentCheck : public ComponentUI
{
public:
	GameObject* parent;

	uint widthPanel;
	uint heigthPanel;

	bool isSelected;

	string selectedImagePath;
	string notSelectedImagePath;

	_functions actualFunction;

	Texture* textureActive;
	Texture* textureDisabled;

public:
	ComponentCheck(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePathActive, const char* imagePathDisabled);
	~ComponentCheck();

	bool OnClick(ComponentUI* UI_Element);

	bool OnIdle(ComponentUI* UI_Element);
	bool OnHover(ComponentUI* UI_Element);
	bool OnClicked();

	void ShowInfo(ComponentUI* UI_Element);
};

#endif // CHECKERUI_H_