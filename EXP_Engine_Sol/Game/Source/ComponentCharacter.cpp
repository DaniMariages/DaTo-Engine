#include "Application.h"
#include "ModuleFonts.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCharacter.h"

ComponentCharacter::ComponentCharacter(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) : 
	ComponentUI(ctype, act, obj, type, w, h, canvas, path, x, y)
{
	text_font = ExternalApp->fonts->defaultFont;
	ch = path;

	panelInGame.textureID = panelInScene.textureID = text_font->characters.at(path[0]).TextureID;

}