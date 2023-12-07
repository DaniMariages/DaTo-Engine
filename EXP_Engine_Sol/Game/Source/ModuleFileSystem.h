#include "Module.h"

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

struct FileInformation
{
	//File name and extension
	std::string fileName;
	std::string fileExtension;
	std::string fileExtensionMinus;

	//File path
	std::string path;
	std::string filePath;
};

class Folder
{
public:
	std::string folderName;
	std::string folderPath;
	std::vector<FileInformation> files;
	std::vector<Folder> folders;

	bool valid = false;
};

class WatchingFloder
{
public:
	std::string folder;
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Awake();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	std::string GetAssetsPath();
	std::string GetLibraryPath();
	std::string GetLibraryMeshPath();
	std::string GetLibraryPrefabPath();
	std::string GetLibraryTexturePath();
	std::string GetLibraryScenePath();
	std::string GetSettingsPath();

	// Get all the file information
	FileInformation GetFileInformation(std::string filePath);

	// Get the name without extension
	std::string FileName(const char* filePath);

	// Get the file extension
	std::string GetFileExtension(const char* filePath);

	// Get the Name with extension
	std::string GetNameWithExtension(const char* filePath);

	// Get the file path
	std::string GetPath(const char* filePath);

	// Get the folder name
	std::string GetFolderName(const char* filePath);

	// Get the parent folder
	std::string GetParentFolder(const char* folderPath);

private:

	// Rename the file if exists
	std::string ReName(const char* filename);

	// Get the number of repeated name. Example: BakerHouse (1) -> 1
	int GetFileNameNumber(const char* filename);

	// Set a new number for a repeated name
	std::string SetFileNameNumber(const char* filename, int number);

	Folder GetFoldersRecursive(const char* path);
	void UnwatchAllFolders();

private:
	std::string assetsPath;
	std::string libraryPath;
	std::string libraryMeshPath;
	std::string libraryPrefabPath;
	std::string libraryTexturePath;
	std::string libraryScenePath;
	std::string settingsPath;

	std::string lookingPath;

	std::vector<WatchingFloder> watchingFolders;
};