#pragma once
#include "ComponentUI.h"

class ComponentCharacter;
class ResourceMesh;
class Font;
class ResourceTexture;

class ComponentLabel :public ComponentUI {
public:
	ComponentLabel(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* text, uint x, uint y);
	~ComponentLabel();

private:
	void CreateText();

public:
	std::string text_str;

};