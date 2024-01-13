#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentImage.h"

ComponentImage::ComponentImage(typeComponent ctype, bool act, GameObject* obj, UI_type type, uint w, uint h, ComponentCanvas* canvas, const char* path, uint x, uint y) :ComponentUI(ctype, act, obj, type, w, h, canvas, path, x, y)
{
	img_path = path;
	panelInGame.textureID = panelInScene.textureID = ExternalApp->importer->GetImageID(path);
}