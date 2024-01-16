#include "ComponentInputText.h"
#include "Application.h"
#include "ModuleImport.h"
#include "ComponentCanvas.h"
#include "Application.h"
#include "ComponentUI.h"
#include "ModuleFont.h"
#include "ComponentText.h"
#include "ModuleScene.h"

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

InputText::InputText(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* Text) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, Text)
{
	parent = gameObject;
	parent->isText = true;
	actualFonts = ARIAL;

	text = Text;

	font = ExternalApp->fonts->actualFont;

	modified = false;

	widthPanel = width;
	heigthPanel = heigt;

	positionX = PosX;
	positionY = PosY;

	actualText = Text;
	newText = Text;

	timeWait.ReStart();
	timeWait.Stop();

	DoText();
}

InputText::~InputText()
{
	parent = nullptr;
	font = nullptr;
	timeWait.ReStart();
	timeWait.Stop();
}

void InputText::Update(ComponentUI* UI_Element)
{
	
}

void InputText::OnClick(ComponentUI* UI_Element)
{
	UI_Element->IsTextEditing = true;
}

void InputText::ShowInfo(ComponentUI* compUI, string actText, string newText, GameObject* gm, FONTS* actFont, uint width, uint heigth, uint _posX, uint _posY)
{
	
}

void InputText::DoText()
{
	
}
void InputText::RecreateText(string new_Text, GameObject* gm, uint width, uint heigth, uint _posX, uint _posY)
{
	
}