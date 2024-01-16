#include "ComponentText.h"
#include "Application.h"
#include "ModuleImport.h"
#include "ComponentCanvas.h"
#include "Application.h"
#include "ComponentUI.h"
#include "ModuleFont.h"

//#include "../External/ImGui/misc/cpp/imgui_stdlib.h"
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

ComponentText::ComponentText(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* Text) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, Text)
{
	parent = gameObject;
	parent->isText = true;
	actualFonts = ARIAL;

	text = Text;

	font = ExternalApp->fonts->actualFont;

	hasBeenModified = false;

	widthPanel = width;
	heigthPanel = heigt;

	positionX = PosX;
	positionY = PosY;

	actualText = Text;
	newText = Text;

	DoText();
}

ComponentText::~ComponentText()
{
	parent = nullptr;
	font = nullptr;
}

void ComponentText::ShowInfo(ComponentUI* compUI, string actText, string newText, GameObject* gm, FONTS* actFont, uint width, uint heigth, uint _posX, uint _posY)
{
	//MYTODO: DISPLAY INFO
}

void ComponentText::DoText()
{
	/*if (text != "")
	{
		uint size_of_character = widthPanel / text.length();

		for (int i = 0; i < text.length(); i++)
		{
			uint position_of_character = positionX + 1 * i;

			ComponentMesh* mesh = (ComponentMesh*)parent->AddComponent(typeComponent::Mesh);

			float3 vertex[4];
			float2 uv[4];
			uint buffer[3];
			float3 transform = { (float)position_of_character, (float)positionY, 0 };

			CreatePanel(vertex, transform, size_of_character, heigthPanel);

			mesh->Name = "Letter";
			mesh->C_Mesh->num_vertex = 4;
			mesh->C_Mesh->num_index = 6;
			mesh->C_Mesh->num_Tex = 2;

			uv[0] = float2(0, 1);
			uv[1] = float2(1, 1);
			uv[3] = float2(1, 0);
			uv[2] = float2(0, 0);

			GenerateBuffers(buffer, vertex, uv);

			mesh->C_Mesh->VBO = buffer[0];
			mesh->C_Mesh->EBO = buffer[1];
			mesh->C_Mesh->VT = buffer[2];
		}
	}*/
}
void ComponentText::RecreateText(string new_Text, GameObject* gm, uint width, uint heigth, uint _posX, uint _posY)
{
	/*gm->DeleteComponentType(ComponentType::MESH);

	if (new_Text != "")
	{
		uint size_of_character = width / new_Text.length();

		for (int i = 0; i < new_Text.length(); i++)
		{
			uint position_of_character = _posX + 1 * i;

			ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(gm->AddComponent(ComponentType::MESH));

			float3 vertex[4];
			float2 uv[4];
			uint buffer[3];
			float3 transform = { (float)position_of_character, (float)_posY, 0 };

			CreatePanel(vertex, transform, size_of_character, heigth);

			mesh->Name = "Letter";
			mesh->C_Mesh->num_vertex = 4;
			mesh->C_Mesh->num_index = 6;
			mesh->C_Mesh->num_Tex = 2;

			uv[0] = float2(0, 1);
			uv[1] = float2(1, 1);
			uv[3] = float2(1, 0);
			uv[2] = float2(0, 0);

			GenerateBuffers(buffer, vertex, uv);

			mesh->C_Mesh->VBO = buffer[0];
			mesh->C_Mesh->EBO = buffer[1];
			mesh->C_Mesh->VT = buffer[2];
		}
	}*/
}
