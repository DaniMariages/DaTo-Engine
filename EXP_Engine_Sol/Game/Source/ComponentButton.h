#pragma once
#include "ComponentUI.h"

class ComponentButton : public ComponentUI {
public:
	ComponentButton(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentButton() {};

	bool UpdateUI(float dt);
	bool OnHover() override;
	bool OnClick() override;

private:
	bool FadeToBlack(float dt);

	std::string img_path;
	bool function = false;
};