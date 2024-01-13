#pragma once
#include "ComponentUI.h"

class ComponentImage :public ComponentUI
{
public:
	ComponentImage(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentImage() {};

private:
	std::string img_path;
};