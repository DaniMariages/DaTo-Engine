#include "ComponentCheck.h"
#include "Application.h"
#include "ComponentTexture.h"

#include "../External/Glew/include/glew.h"
#include "../External/ImGui/imgui.h"
#include "../External/ImGui/backends/imgui_impl_sdl2.h"
#include "../External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "../External/SDL/include/SDL_opengl.h"
#endif

ComponentCheck::ComponentCheck(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePathActive, const char* imagePathDisabled) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePathActive)
{
	parent = gameObject;
	actualFunction = _functions::_DEF;
	positionX = PosX;
	positionY = PosY;
	widthPanel = width;
	heigthPanel = heigt;
}

ComponentCheck::~ComponentCheck()
{
	parent = nullptr;
}

bool ComponentCheck::OnClicked()
{

	return true;
}

bool ComponentCheck::OnIdle(ComponentUI* UI_Element)
{
	if (!UI_Element->isSelected)
	{
		color = float4(1, 1, 1, 1);

		/*	float col[4] = { 0.5, 0.5, 0.5, 0.5 };*/
		ComponentTexture* mat = dynamic_cast<ComponentTexture*>(UI_Element->parent->GetComponent(typeComponent::Material));
		mat->texColor.r = 1;
		mat->texColor.g = 1;
		mat->texColor.b = 1;
		mat->texColor.a = 1;
	}

	return false;
}

bool ComponentCheck::OnHover(ComponentUI* UI_Element)
{
	color = float4(0.5, 0.5, 0.5, 0.5);

	ComponentTexture* mat = dynamic_cast<ComponentTexture*>(UI_Element->parent->GetComponent(typeComponent::Material));
	mat->texColor.r = 0;
	mat->texColor.g = 1;
	mat->texColor.b = 0;
	mat->texColor.a = 1;

	return true;
}

bool ComponentCheck::OnClick(ComponentUI* UI_Element)
{
	if (ExternalApp->scene->gameCamera != nullptr)
	{
		switch (UI_Element->actualChecker)
		{
		case _functions::_VSYNC:
			UI_Element->CheckSelected = !UI_Element->CheckSelected;
			ExternalApp->editor->Vsync = UI_Element->CheckSelected;
			break;
		case _functions::_DRAG:
			UI_Element->CheckSelected = !UI_Element->CheckSelected;
			ExternalApp->scene->draggable = UI_Element->CheckSelected;
			break;
		default:
			break;
		}
	}

	ComponentTexture* mat = dynamic_cast<ComponentTexture*>(UI_Element->parent->GetComponent(typeComponent::Material));

	if (UI_Element->CheckSelected)
	{
		if (UI_Element->active != nullptr)
		{
			mat->Disable();
		}
	}
	else
	{
		if (UI_Element->disabled != nullptr)
		{
			mat->Enable();
		}
	}

	return UI_Element->CheckSelected;
}

void ComponentCheck::ShowInfo(ComponentUI* UI_Element)
{
	if (ImGui::TreeNode("Checker"))
	{
		ImGui::Checkbox("Active", &UI_Element->parent->active);

		ComponentTexture* mat = dynamic_cast<ComponentTexture*>(UI_Element->parent->GetComponent(typeComponent::Material));

		if (UI_Element->active != nullptr && UI_Element->disabled != nullptr)
		{
			ImGui::Image((void*)texture->textID, ImVec2(texture->width, texture->height));
			ImGui::Text("Texture path: %s", texture->path);
			ImGui::Text("Texture ID: %d", texture->textID);
			ImGui::Text("Texture Size: Width:%d x Height:%d", texture->width, texture->height);

			mat->texColor.a = 0;
		}
		else
		{
			mat->texColor.a = 255;
		}

		float col[4] = { mat->texColor.r, mat->texColor.g, mat->texColor.b, mat->texColor.a };

		ImGui::ColorEdit4("Material Color", col);

		mat->texColor.r = col[0];
		mat->texColor.g = col[1];
		mat->texColor.b = col[2];
		mat->texColor.a = col[3];

		if (ImGui::Button("Action"))
		{
			ImGui::OpenPopup("Action");
		}

		switch (UI_Element->actualChecker)
		{
		case _functions::_VSYNC:
			ImGui::Text("Actual action: Vsync");
			break;
		case _functions::_DRAG:
			ImGui::Text("Actual action: Drag");
			break;
		case _functions::_DEF:
			ImGui::Text("Actual action: None");
			break;
		default:
			ImGui::Text("Actual action: None");
			break;
		}

		values* namesOfValues = new values;

		if (ImGui::BeginPopup("Action"))
		{
			for (int j = 0; j < (_functions::_MAX); j++)
			{
				if (ImGui::MenuItem((namesOfValues->nameOfFunctions[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
				{
					switch (j)
					{
					case (int)_functions::_VSYNC:
						UI_Element->actualChecker = _VSYNC;
						break;
					case (int)_functions::_DRAG:
						UI_Element->actualChecker = _DRAG;
						break;
					case (int)_functions::_DEF:
						UI_Element->actualChecker = _DEF;
						break;
					default:
						break;
					}
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Checkbox("State", &UI_Element->CheckSelected);

		ImGui::TreePop();
	}
}