#include "Application.h"
#include "ComponentCanvas.h"
#include "ComponentButton.h"
#include "GameObject.h"
#include "ComponentMesh.h"

ComponentButton::ComponentButton(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) 
	: ComponentUI(ctype, act, obj, type, w, h, canvas, path, x, y)
{
	img_path = path;
	panelInGame.textureID = panelInScene.textureID = ExternalApp->importer->GetImageID(path);
}

bool ComponentButton::UpdateUI(float dt)
{
	if (function)
	{
		function = !FadeToBlack(dt);

		/*if (!function)
			App->viewport->to_load_scene01 = true;*/
	}

	return true;
}

bool ComponentButton::OnHover()
{
	if (!dragable)
		fill_color = float4(0.5, 0.5, 0.5, fill_color.w);
	return true;
}

bool ComponentButton::OnClick()
{
	if (!dragable)
	{
		fill_color = float4(0.2, 0.2, 0.2, fill_color.w);
		if (parent->Name == "StartButton")
			function = true;
		else if (parent->Name == "CloseButton")
		{
			state = UI_Idle;
		}
	}

	return true;
}

bool ComponentButton::FadeToBlack(float dt)
{
	bool ret = false;

	for (auto it_go = canvas->parent->children.begin(); it_go != canvas->parent->children.end(); it_go++)
	{
		if ((*it_go)->GetComponent(typeComponent::UI))
		{
			ComponentUI* compUI = (ComponentUI*)(*it_go)->GetComponent(typeComponent::UI);
			compUI->Fade();
		}
	}

	return ret;
}