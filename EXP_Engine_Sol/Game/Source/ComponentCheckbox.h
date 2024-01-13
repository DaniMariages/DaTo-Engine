#pragma once
#include "ComponentUI.h"

class ComponentCheckbox : public ComponentUI
{
public:
	ComponentCheckbox(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y);
	~ComponentCheckbox() {};

	bool OnClick() override;

	void SetState(bool st);

	void SetDragable();

private:
	std::string checkbox_name;
	bool bool_state = false;

	uint checkbox_false = 0;
	uint checkbox_true = 0;
};