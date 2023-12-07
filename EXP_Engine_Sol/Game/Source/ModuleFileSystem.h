#include "Module.h"

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#define ASSETS_PATH "Assets/"

#define LIBRARY_PATH "Library/"
#define MESHES_PATH "Library/Meshes/"
#define MATERIALS_PATH "Library/Materials/"
#define MODELS_PATH "Library/Models/"
#define SCENES_PATH "Library/Scenes/"

enum class importedFileType
{
	MODEL,
	TEXTURE,

	UNKNOWN,
};

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

	//Create all the library folders
	void CreateFolders();

	std::string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = false);
	bool FileCopyPaste(const char* filepath, const char* new_path, bool overwrite, std::string& resultant_path);
	bool FileCopyPaste(const char* filepath, const char* new_path, bool overwrite);
	void FileCopyPasteWithNewName(const char* filepath, const char* new_path, const char* new_name);
	bool FileDelete(const char* filepath);
	bool FolderDelete(const char* folderpath);
	bool FileSave(const char* path, const char* file_content, const char* name, const char* extension, int size);
	std::vector<std::string> GetFilesAndFoldersInPath(const char* path, const char* extension = "");
	std::vector<std::string> GetFoldersInPath(const char* path);
	std::vector<std::string> GetFilesInPath(const char* path, const char* extension = "");
	std::vector<std::string> GetFilesInPathAndChilds(const char* path);

	Folder GetFilesAndFoldersTree(const char* path);

	bool FileExists(const char* path, const char* name, const char* extension = "");
	bool FileExists(const char* filepath);
	bool FileRename(const char* filepath, const char* new_name);
	bool FolderRename(const char* filepath, const char* new_name);
	bool FolderExists(const char* path);
	std::string FileRenameOnNameCollision(const char* path, const char* name, const char* extension);

	//Useful fuctions
	std::string ToLowerCase(std::string str);
	bool TextCmp(const char* text1, const char* text2);

	std::string GetAssetsPath();
	std::string GetLibraryPath();
	std::string GetLibraryMeshPath();
	std::string GetLibraryPrefabPath();
	std::string GetLibraryTexturePath();
	std::string GetLibraryScenePath();
	std::string GetLibraryShadersPath();
	std::string GetSettingsPath();

	std::string GetLookingPath();
	void SetLookingPath(const std::string& new_path);

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
	// Same name file renaming
	std::string NewNameForFileNameCollision(const char* filename);
	int GetFileNameNumber(const char* filename);
	std::string SetFileNameNumber(const char* filename, int number);


	Folder GetFoldersRecursive(const char* path);
	void UnwatchAllFolders();

private:
	std::string assets_path;
	std::string library_path;
	std::string library_mesh_path;
	std::string library_prefab_path;
	std::string library_texture_path;
	std::string library_scene_path;
	std::string library_shaders_path;
	std::string settings_path;

	std::string looking_path;

	std::vector<WatchingFloder> watching_folders;
};