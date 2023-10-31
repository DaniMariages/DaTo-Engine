#pragma once
#include "Module.h"
#include "Globals.h"

#include "../External/MathGeoLib/include/MathGeolib.h"

#include <vector>

class ModuleMesh : public Module
{
public:
	struct Vertex {
		float3 Position;
		float3 Normal;
		float2 TexCoords;
	};

	struct mesh {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int VBO = 0, EBO = 0;
	};
	std::vector<mesh> meshes;

	ModuleMesh(Application* app, bool start_enabled = true);
	~ModuleMesh();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LoadMesh(const char* file_path);
};
