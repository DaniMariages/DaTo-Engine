#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "Primitive.h"
#include "ModuleScene.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Math/float4x4.h"
#include "../External/Glew/include/glew.h"

#define MAX_LIGHTS 8
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

struct mesh;

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

	void UseCheckerTexture();
	
	void IterateDrawMesh();
	void DrawMesh(mesh* mesh, float4x4 transform, uint id = 0);
	void DrawFaceNormals(mesh* Mesh, float3 position, float3 scale, Quat rotation);
	void DrawVertexNormals(mesh* Mesh, float3 position, float3 scale, Quat rotation);
	
	//Draw normals of a selected game object
	void DrawSelectedNormals();

	//Import primitives
	void ImportCube();
	void ImportSphere();
	void ImportCone();
	void ImportCylinder();
	void ImportTorus();

	//To draw the face normal
	float3 CalculateFaceNormal(const float3& vertex1, const float3& vertex2, const float3& vertex3);

	//To draw a box
	void DrawBox(float3* vertices, float3 color);

	//Function to draw the bounding box of a game object
	void RenderBB();

	//Bool to know if a AABB is inside the FOV camera
	bool InsideCamera(const ComponentCamera* camera, const AABB& aabb);

	//Debug the ray of editor camera
	void RenderRay();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	GLuint checkersTexture;
	GLuint checkerID;
	GLuint texWidth, texHeight;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
};