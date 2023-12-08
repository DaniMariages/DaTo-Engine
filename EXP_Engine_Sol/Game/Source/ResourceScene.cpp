#include "ResourceScene.h"
#include "GameObject.h"

ResourceScene::ResourceScene() : Resource(ResourceType::Scene)
{
	//rootObject = new GameObject("Scene");
	//models.clear();
}

ResourceScene::ResourceScene(const char* assetsFile, const char* libraryFile, const char* name, uint UID) : Resource(ResourceType::Mesh)
{
	this->name = name;
	this->libraryFile = libraryFile;
	this->assetsFile = assetsFile;
	if (UID != 0) this->UID = UID;
}

ResourceScene::~ResourceScene()
{
}
