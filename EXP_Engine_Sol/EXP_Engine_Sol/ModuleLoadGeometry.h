#pragma once

#include "Assimp/include/cimport.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/scene.h"
using namespace std;

#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ModuleLoadGeometry : public Module
{
public:
	ModuleLoadGeometry(Application* app, bool start_enabled = true);
	~ModuleLoadGeometry();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadFile();
	void LoadMeshes();

	const char* file_path = "C:/Users/antoniorr14/Documents/GitHub/Motors/EXP_Engine_Sol/EXP_Engine_Sol/Assets_FBX/warrior.FBX";
	size_t size = 0;
	
private:
	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
	};

	class Mesh {
	public:
		// mesh data
		vector<Vertex>   vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		//void Draw(Shader& shader);

	private:
		//  render data
		unsigned int VAO, VBO, EBO;

		void setupMesh();
	};

};