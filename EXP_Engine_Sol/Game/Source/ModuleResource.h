#pragma once
#include "Module.h"
#include "Globals.h"
#include "Resource.h"

#include <map>

class Random;
class ResourceMesh;
class ResourceTexture;
class ResourceScene;

struct PathNode;

class ModuleResource : public Module
{
public:
	ModuleResource(Application* app, bool start_enabled = true);
	~ModuleResource();

	bool Start() override;
	bool CleanUp() override;

	void LoadAssets();
	bool IterateAssets(PathNode node, uint ID);
	void LoadScene(const char* buffer, uint size, ResourceScene* scene);
	uint Find(const char* file);
	uint ImportFile(const char* file_assets);
	ResourceType GetTypeFromFile(const char* path) const;
	ResourceType GetTypefromString(std::string) const;
	std::string GetStringFromResource(Resource* resource);

	Resource* CreateNewResource(const char* assetsFile, ResourceType type, const char* name = "", uint UID = 0);
	void SaveMeta(Resource* resource);
	void SaveResource(Resource* resource);
	Resource* LoadResource(uint UID, Resource* resource = nullptr);
	Resource* AccesResource(uint UID);
	void ReleaseResource(uint UID);
	//uint32 UnloadResource(uint32 ID);

	inline uint GenerateNewUID() { return randomNum.GenerateRandomInt(); }

public:

	std::map<uint, Resource*> resources;

private:

	std::map<uint, Resource*> importedResources;


	Random randomNum;
};
