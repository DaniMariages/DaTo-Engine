#pragma once
#include "Component.h"
#include "ComponentUI.h"
#include "Color.h"
#include "ModuleFont.h"
#include "ComponentMesh.h"
#include "Timer.h"

#include <vector>
#include <functional>

class GameObject;

class InputText : public ComponentUI
{
public:
	GameObject* parent;

	uint widthPanel;
	uint heigthPanel;

	string text;
	string actualText;
	string newText;

	Font* font;

	bool hasBeenModified;

	FONTS actualFonts;

	Timer timeWait;

public:
	InputText(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~InputText();

	void ShowInfo(ComponentUI* compUI, string actText, string newText, GameObject* gm, FONTS* actFont, uint width, uint heigth, uint _posX, uint _posY);

	void DoText();

	void OnClick(ComponentUI* UI_Element);

	void Update(ComponentUI* UI_Element);

	void RecreateText(string new_Text, GameObject* gm, uint width, uint heigth, uint _posX, uint _posY);
};