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

	modified = false;

	widthPanel = width;
	heigthPanel = heigt;

	positionX = PosX;
	positionY = PosY;

	actualText = Text;
	newText = Text;

	DoText();
}

ComponentText::ComponentText(GameObject* _parent) : ComponentUI(_parent)
{
	parent = _parent;
	parent->isText = true;
	actualFonts = ARIAL;

	text = "ComponentText";
	font = ExternalApp->fonts->actualFont;

	modified = false;
	widthPanel = 150;
	heigthPanel = 150;

	positionX = 300;
	positionY = 300; 

	actualText = "ComponentText";
	newText = "ComponentText";

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
	
}
void ComponentText::RecreateText(string new_Text, GameObject* gm, uint width, uint heigth, uint _posX, uint _posY)
{
	
}
