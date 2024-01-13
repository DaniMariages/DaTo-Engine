#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentLabel.h"
#include "ModuleScene.h"

ComponentLabel::ComponentLabel(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* text, uint x, uint y) : ComponentUI(ctype, act, obj, type, w, h, canvas, text, x, y)
{
	text_str = text;
	CreateText();
}

ComponentLabel::~ComponentLabel()
{
}

void ComponentLabel::CreateText()
{
	ComponentTransform* comp_trans = (ComponentTransform*)parent->GetComponent(typeComponent::Transform);
	uint width_ch = width / text_str.length();
	for (int i = 0; i < text_str.length(); i++)
	{
		uint pos_aux_x = pos_x + width_ch * i;
		GameObject* char_aux = ExternalApp->scene->CreateGameObject(std::string(text_str.substr(i, 1) + "_") + text_str, parent);
		char_aux->CreateComponentUI(UI_Character, width_ch, height, canvas, text_str.substr(i, 1).c_str());
	}
}