#pragma once
#include "Module.h"
#include "Globals.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

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

	Texture(GLuint id, uint _width, uint _height, const char* _path)
	{
		textID = id;
		width = _width;
		height = _height;
		path = _path;
	}
	Texture() {};
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
	unsigned int VBO = 0, EBO = 0;

	enum  Buffers
	{
		index,
		vertex,
		normal,
		texture,
		NONE
	};
	uint ID[NONE];
	uint size[NONE];
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
	std::string path;
	std::string name;
};
