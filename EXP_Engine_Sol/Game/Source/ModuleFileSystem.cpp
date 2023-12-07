#include "ModuleFileSystem.h"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleFileSystem::~ModuleFileSystem() {}

bool ModuleFileSystem::Awake() { return true; }

bool ModuleFileSystem::Start() { return true; }

bool ModuleFileSystem::CleanUp() { return true; }

update_status ModuleFileSystem::Update(float dt) { return UPDATE_CONTINUE; }


// GETTERS ---------------------------------------------
std::string ModuleFileSystem::GetNameWithExtension(const char* filename)
{
	std::string name(filename);
	size_t lastSeparator = name.find_last_of("/\\");

	if (lastSeparator != std::string::npos)
	{
		name = name.substr(lastSeparator + 1);
	}
	else name = name;

	LOG("The file name is: %s", name.c_str());
	return name;
}

int ModuleFileSystem::GetFileNameNumber(const char* filename)
{
	int num = -1;
	std::string name(filename);
	size_t firstParentheses = name.find_first_of("(");
	size_t lastParentheses = name.find_first_of(")");

	if (firstParentheses != std::string::npos)
	{
		name = name.substr(firstParentheses + 1 - lastParentheses);
	}
	else LOG("The name is not repeated!");

	num = atoi(name.c_str());

	return num;
}

std::string ModuleFileSystem::GetAssetsPath() { return assetsPath; }

std::string ModuleFileSystem::GetLibraryPath() { return libraryPath; }

std::string ModuleFileSystem::GetLibraryMeshPath() { return libraryMeshPath; }

std::string ModuleFileSystem::GetLibraryPrefabPath() { return libraryPrefabPath; }

std::string ModuleFileSystem::GetLibraryTexturePath() { return libraryTexturePath; }

std::string ModuleFileSystem::GetLibraryScenePath() { return libraryScenePath; }

std::string ModuleFileSystem::GetSettingsPath() { return settingsPath; }