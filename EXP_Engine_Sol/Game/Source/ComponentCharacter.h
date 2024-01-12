#pragma once
#include "ComponentUI.h"

class ComponentCharacter :public ComponentUI
{
public:
	ComponentCharacter(typeComponent compType, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentCharacter() {};

private:
	std::string ch;
	//Font* text_font = nullptr;
};