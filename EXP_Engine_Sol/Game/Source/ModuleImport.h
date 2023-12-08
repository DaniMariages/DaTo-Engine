//#pragma once
//#include "Module.h"
//#include "Globals.h"
//
//#include "GameObject.h"
//#include "ComponentMesh.h"
//#include "ComponentTexture.h"
//#include "ComponentTransform.h"
//
//#include "../External/Assimp/include/cimport.h"
//#include "../External/Assimp/include/scene.h"
//#include "../External/Assimp/include/postprocess.h"
//
//#include "../External/MathGeoLib/include/MathGeolib.h"
//
//#include "../External/DevIL/include/il.h"
//#include "../External/DevIL/include/ilu.h"
//#include "../External/DevIL/include/ilut.h"
//
//#include <vector>
//
//enum class typeFile
//{
//	MODEL, 
//	TEXTURE,
//
//	UNKNOWN,
//};
//
//enum class typeOfGO
//{
//	CHILD_OF_OBJECT,
//	CHILD_OF_SCENE,
//
//	UNKNOWN,
//};
//
//struct Texture
//{
//	GLuint textID;
//	const char* path;
//};
//
//struct Vertex
//{
//	float3 Position;
//	float3 Normal;
//	float2 TexCoords;
//};
//
//struct mesh
//{
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//	unsigned int VBO = 0;
//	unsigned int EBO = 0;
//};
//
//
//class ModuleImport : public Module
//{
//public:
//	
//	std::vector<mesh> meshes;
//
//	ModuleImport(Application* app, bool start_enabled = true);
//	~ModuleImport();
//
//	bool Start();
//	update_status Update(float dt);
//	bool CleanUp();
//
//	void ReadFile(const char* filePath);
//
//	//Function that check if the name exists in game objects, if exists, call ReName.
//	std::string GetUniqueName(std::string Name);
//
//	//Set up the buffers for meshes
//	void SetUpBuffers(const mesh* Mesh);
//
//private:
//	
//	//Get the name of game object from the file path.
//	std::string GetName(const char* filePath);
//
//	//Rename the game object if its necessary (called from GetUniqueName).
//	std::string ReName(std::string Name, int counter);
//
//	typeFile ReadExtension(std::string filePath);
//
//	void LoadMesh(const char* filePath);
//	Texture* LoadTexture(const char* filePath);
//
//	mesh ProcessMesh(aiMesh* Mesh, const char* filePath, GameObject* gameObject);
//	void GetSceneInfo(aiNode* node, const aiScene* scene, const char* filePath, GameObject* gameObject);
//
//	GameObject* newGameObject = nullptr;
//	typeOfGO GO = typeOfGO::UNKNOWN;
//
//	std::string path;
//	std::string name;
//
//};
