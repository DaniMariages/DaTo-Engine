#pragma once
#include "Module.h"
#include "Globals.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"

#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"

#include "../External/MathGeoLib/include/MathGeolib.h"

#include "../External/DevIL/include/il.h"
#include "../External/DevIL/include/ilu.h"
#include "../External/DevIL/include/ilut.h"

#include <vector>

enum class typeFile
{
	MODEL, 
	TEXTURE,

	UNKNOWN,
};

enum class typeOfGO
{
	CHILD_OF_OBJECT,
	CHILD_OF_SCENE,

	UNKNOWN,
};

struct Texture
{
	GLuint textID;
	uint width;
	uint height;
	const char* path;
};

struct Vertex
{
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};


struct mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};


class ModuleImport : public Module
{
public:
	
	std::vector<mesh> meshes;

	ModuleImport(Application* app, bool start_enabled = true);
	~ModuleImport();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ReadFile(const char* file_path);

private:
	std::string GetName(const char* file_path);
	void GetUniqueName(std::string Name);
	void ReName(std::string Name, int counter);

	typeFile ReadExtension(std::string file_path);

	void LoadMesh(const char* file_path);
	Texture* LoadTexture(const char* file_path);

	mesh ProcessMesh(aiMesh* Mesh, const char* file_path, GameObject* gameObject);
	void GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject);

	GameObject* newGameObject = nullptr;
	typeOfGO GO = typeOfGO::UNKNOWN;

	std::string path;
	std::string name;
	int counter = 0;
};
