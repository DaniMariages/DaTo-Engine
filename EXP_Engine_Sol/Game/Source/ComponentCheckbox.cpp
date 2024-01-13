#include "Application.h"
#include "GameObject.h"
#include "ComponentCheckbox.h"
#include "Component.h"
#include "ComponentMesh.h"

ComponentCheckbox::ComponentCheckbox(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) :ComponentUI(ctype, act, obj, type, w, h, canvas, path, x, y)
{
	checkbox_name = path;
	panelInGame.textureID = panelInScene.textureID = checkbox_false = ExternalApp->importer->GetImageID("./Assets/checkbox.png");
	checkbox_true = ExternalApp->importer->GetImageID("./Assets/checkboxOn.png");
}

bool ComponentCheckbox::OnClick()
{
	if (dragable && checkbox_name != "Draggable")
		return true;

	if (bool_state)
	{
		panelInScene.textureID = checkbox_false;
		panelInGame.textureID = checkbox_false;
		bool_state = false;
	}
	else
	{
		panelInScene.textureID = checkbox_true;
		panelInGame.textureID = checkbox_true;
		bool_state = true;
	}

	if (checkbox_name == "Draggable")
	{
		SetDragable();
		LOG("Draggable set to: %i", bool_state);
	}
	else if (checkbox_name == "Vsync")
	{
		//ExternalApp->window->SetVsync(bool_state);
		LOG("Vsync set to: %i", bool_state);
	}

	return true;
}

void ComponentCheckbox::SetState(bool st)
{
	if (st)
	{
		panelInGame.textureID = panelInScene.textureID = checkbox_true;
	}
	else panelInGame.textureID = panelInScene.textureID = checkbox_false;

	bool_state = &st;
}

void ComponentCheckbox::SetDragable()
{
	for (auto it_go = canvas->parent->children.begin(); it_go != canvas->parent->children.end(); it_go++)
	{
		if ((*it_go)->GetComponent(typeComponent::UI))
		{
			ComponentUI* compUI = (ComponentUI*)(*it_go)->GetComponent(typeComponent::UI);
			compUI->dragable = bool_state;
		}
	}
}