#pragma once
#include "Module.h"
#include "Globals.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"

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
	typeFile ReadExtension(std::string file_path);

	void LoadMesh(const char* file_path);
	Texture* LoadTexture(const char* file_path);

	GameObject* BakerHouse = nullptr;

	std::string path;
	std::string name;
};
