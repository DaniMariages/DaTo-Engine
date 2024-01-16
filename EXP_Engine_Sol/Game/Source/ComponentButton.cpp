#include "ComponentButton.h"
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

ComponentButton::ComponentButton(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePath)
{
	parent = gameObject;
	positionX = PosX;
	positionY = PosY;
	widthPanel = width;
	heigthPanel = heigt;

	isPressed = false;
}

ComponentButton::~ComponentButton()
{
}

bool ComponentButton::OnClicked()
{

	return true;
}

bool ComponentButton::OnIdle(ComponentUI* UI_Element)
{
	if (!UI_Element->isSelected)
	{
		color = float4(1, 1, 1, 1);

		ComponentTexture* mat = (ComponentTexture*)parent->GetComponent(typeComponent::Material);
		mat->texColor.r = 1;
		mat->texColor.g = 1;
		mat->texColor.b = 1;
		mat->texColor.a = 1;
	}

	return false;
}

bool ComponentButton::OnHover(ComponentUI* UI_Element)
{
	color = float4(0.5, 0.5, 0.5, 0.5);

	ComponentTexture* mat = (ComponentTexture*)parent->GetComponent(typeComponent::Material);
	mat->texColor.r = 0;
	mat->texColor.g = 1;
	mat->texColor.b = 0;
	mat->texColor.a = 1;

	return true;
}

bool ComponentButton::OnClick()
{
	PassScene();
	return isPressed;
}

void ComponentButton::ShowInfo(int* action)
{
	if (ImGui::TreeNode("Button"))
	{
		ImGui::Checkbox("Active", &parent->active);

		if (texture != nullptr)
		{
			ImGui::Image((void*)texture->textID, ImVec2(texture->width, texture->height));
			ImGui::Text("Texture path: %s", texture->path);
			ImGui::Text("Texture ID: %d", texture->textID);
			ImGui::Text("Texture Size: Width:%d x Height:%d", texture->width, texture->height);

			color[3] = 0;
		}
		else
		{
			color[3] = 50;
		}

		float col[4] = { color[0], color[1], color[2], color[3] };

		ImGui::ColorEdit4("Material Color", col);

		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
		color[3] = col[3];
	}
}

void ComponentButton::PassScene()
{
	LOG("PASS SCENE");
}

void ComponentButton::CreatePauseMenu()
{
	LOG("PAUSE MENU")
}
