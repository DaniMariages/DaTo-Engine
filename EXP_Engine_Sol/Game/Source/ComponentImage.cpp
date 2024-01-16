#include "ComponentImage.h"
#include "Application.h"
#include "ModuleImport.h"
#include "ModuleScene.h"
#include "ComponentCanvas.h"

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

ComponentImage::ComponentImage(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePath)
{
	gmAtached = gameObject;

	image_Path = new Texture();

	scaleToCanvas = false;
}

ComponentImage::~ComponentImage()
{
	gmAtached = nullptr;
	delete image_Path;
	image_Path = nullptr;
}

void ComponentImage::ShowInfo()
{
	ComponentTexture* compTex = (ComponentTexture*)parent->GetComponent(typeComponent::Material);

	if (compTex->GetTexture() != nullptr)
	{
		if (compTex->GetTexture()->textID != 0)
		{
			image_Path = compTex->GetTexture();
		}

		color[3] = 0;
	}
	else
	{
		color[3] = 255;
	}

	if (scaleToCanvas)
	{
		ComponentCanvas* canv = (ComponentCanvas*)ExternalApp->scene->Canvas->GetComponent(typeComponent::Canvas);
		widthPanel = canv->widthPanel;
		heigthPanel = canv->heigthPanel;
	}

	if (ImGui::TreeNode("Image"))
	{
		ImGui::Checkbox("Active", &parent->active);

		ImGui::Text("Actual Image:");

		if (compTex->GetTexture() != nullptr)
		{
			if (compTex->GetTexture()->textID != 0)
			{
				ImGui::Image((void*)compTex->GetTexture()->textID, ImVec2(compTex->GetTexture()->width, compTex->GetTexture()->height));
			}
			else
			{
				ImGui::Text("No image selected in Texture Component");
			}
		}

		ImGui::Separator();
		ImGui::Checkbox("Scale image to canvas", &scaleToCanvas);

		ImGui::TreePop();
	}
}