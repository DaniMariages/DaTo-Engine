#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "ModuleEditor.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"
#include "../External/Glew/include/glew.h"
#include "Model.h"
#include "ModuleTexture.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void DrawNormals(std::vector<Mesh> meshes);
	void BindVBO();
	float3 CalculateFaceNormal(const float3& vertex1, const float3& vertex2, const float3& vertex3);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	std::vector<Model> Models;
	bool modelLoaded = false;
	const char* myModelPath;

	Texture* tex;
	Mesh* bakerhouse;

	GLuint checkersTexture;
	GLuint checkerID;
	GLuint texWidth, texHeight;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
	uint VBO;
	uint EBO;
	uint VAO;
};