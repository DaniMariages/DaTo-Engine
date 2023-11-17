#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "../External/SDL/include/SDL_opengl.h"
#include "ModuleEditor.h"
#include "ModuleImport.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */	
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Game/External/Glew/libx86/glew32.lib")

#include "../External/ImGui/imgui.h"
#include "../External/ImGui/backends/imgui_impl_sdl2.h"
#include "../External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#pragma comment (lib, "Game/External/MathGeoLib/libx86/M_Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "Game/External/MathGeoLib/libx86/M_Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
static const GLfloat CubeVertices[] = {
	-1, -1, -1,
	1, -1, -1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	-1, 1, 1
};
static const GLuint CubeIndices[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};
// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glewInit();
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	App->editor->Init();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
	ImGui_ImplOpenGL3_Init("#version 130");

	UseCheckerTexture();
	Grid.axis = true;

	ilInit();

	App->importer->ReadFile("Assets/Models/BakerHouse.fbx");
	App->importer->ReadFile("Assets/Textures/BakerHouse.png");

	gOselected = gameObjects[0];

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
			lights[i].Render();

	for (uint n = 0; n < gameObjects.size(); n++)
	{
		gameObjects[n]->Update();
	}


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Render Editor
	Grid.Render();

	IterateDrawMesh();

	glEnd();
	glLineWidth(1.0f);

	App->editor->DrawEditor();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::UseCheckerTexture() {

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerID);
	glBindTexture(GL_TEXTURE_2D, checkerID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetUpBuffers(mesh* mesh)
{
	glGenBuffers(1, &mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->vertices.size(), &mesh->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), &mesh->indices[0], GL_STATIC_DRAW);

	//TexCoords?

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::IterateDrawMesh()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->GetComponent(typeComponent::Mesh) != nullptr)
		{
			std::vector<Component*> meshComponents = gameObjects[i]->GetComponents(typeComponent::Mesh);
			std::vector<Component*>::iterator item = meshComponents.begin();
			for (; item != meshComponents.end(); ++item) 
			{
				ComponentTexture* componentTex = (ComponentTexture*)gameObjects[i]->GetComponent(typeComponent::Material);
				ComponentMesh* tempComponentMesh = (ComponentMesh*)(*item);
				if (componentTex != nullptr && gameObjects[i]->active && App->editor->drawAll)
				{
					DrawMesh(tempComponentMesh->GetMesh(), componentTex->GetTexture()->textID);
					if (App->editor->drawAllFaces == true) DrawFaceNormals(tempComponentMesh->GetMesh());
					if (App->editor->drawAllVertex == true) DrawVertexNormals(tempComponentMesh->GetMesh());
				}
				else if (gameObjects[i]->active && App->editor->drawAll)
				{
					DrawMesh(tempComponentMesh->GetMesh());
					if (App->editor->drawAllFaces == true) DrawFaceNormals(tempComponentMesh->GetMesh());
					if (App->editor->drawAllVertex == true) DrawVertexNormals(tempComponentMesh->GetMesh());
				}

				if (App->editor->drawSelectedFaces)
				{
					std::vector<Component*> meshComp = gameObjects[App->editor->posOfSelected]->GetComponents(typeComponent::Mesh);
					std::vector<Component*>::iterator it = meshComp.begin();
					for (; it != meshComp.end(); ++it)
					{
						ComponentMesh* tempCompMesh = (ComponentMesh*)(*item);
						DrawFaceNormals(tempCompMesh->GetMesh());
					}
				}
				if (App->editor->drawSelectedVertex)
				{
					std::vector<Component*> meshComp = gameObjects[App->editor->posOfSelected]->GetComponents(typeComponent::Mesh);
					std::vector<Component*>::iterator it = meshComp.begin();
					for (; it != meshComp.end(); ++it)
					{
						ComponentMesh* tempCompMesh = (ComponentMesh*)(*item);
						DrawVertexNormals(tempCompMesh->GetMesh());
					}
				}
			}
		}
	}
}

void ModuleRenderer3D::DrawMesh(mesh* mesh, uint id)
{
	// Bind the VBO and EBO for the mesh
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);

	// Enable vertex and normal arrays and specify the pointers
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// Enable the texture coordinate array and specify the pointer
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	if (App->editor->drawTextures)
	{
		// Bind the texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	
	// Draw the mesh using glDrawElements
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VBO, EBO, and texture
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable vertex, normal, and texture coordinate arrays
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void ModuleRenderer3D::DrawFaceNormals(mesh* Mesh)
{
	for (unsigned int i = 0; i < Mesh->indices.size(); i += 3)
	{
		const unsigned int idx1 = Mesh->indices[i];
		const unsigned int idx2 = Mesh->indices[i + 1];
		const unsigned int idx3 = Mesh->indices[i + 2];

		const float3 vertex1 = Mesh->vertices[idx1].Position;
		const float3 vertex2 = Mesh->vertices[idx2].Position;
		const float3 vertex3 = Mesh->vertices[idx3].Position;

		// Calculate the face normal
		float3 faceNormal = CalculateFaceNormal(vertex1, vertex2, vertex3);

		// Calculate the face centroid
		float3 centroid = (vertex1 + vertex2 + vertex3) / 3.0f;

		// Draw the face normal line
		glBegin(GL_LINES);
		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
		glVertex3f(centroid.x, centroid.y, centroid.z);
		glVertex3f(centroid.x + 1.0f * faceNormal.x, centroid.y + 1.0f * faceNormal.y, centroid.z + 1.0f * faceNormal.z);
		glEnd();
	}
}

float3 ModuleRenderer3D::CalculateFaceNormal(const float3& vertex1, const float3& vertex2, const float3& vertex3)
{
	float3 edge1 = vertex2 - vertex1;
	float3 edge2 = vertex3 - vertex1;
	return Cross(edge1, edge2).Normalized();
}

void ModuleRenderer3D::DrawVertexNormals(mesh* Mesh)
{
	for (unsigned int j = 0; j < Mesh->vertices.size(); ++j)
	{
		const float3 vertex = Mesh->vertices[j].Position;
		const float3 normal = Mesh->vertices[j].Normal;

		// Draw the vertex normal line
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex3f(vertex.x, vertex.y, vertex.z);
		glVertex3f(vertex.x + 1.0f * normal.x, vertex.y + 1.0f * normal.y, vertex.z + 1.0f * normal.z);
		glEnd();
	}
}

void ModuleRenderer3D::DrawSelectedNormals(GameObject* gObject)
{
	std::vector<Component*> meshComponents = gObject->GetComponents(typeComponent::Mesh);
	std::vector<Component*>::iterator item = meshComponents.begin();
	for (; item != meshComponents.end(); ++item)
	{
		ComponentTexture* componentTex = (ComponentTexture*)gObject->GetComponent(typeComponent::Material);
		ComponentMesh* tempComponentMesh = (ComponentMesh*)(*item);
		if (componentTex != nullptr && gObject->active)
		{
			if (App->editor->drawSelectedFaces)	 DrawFaceNormals(tempComponentMesh->GetMesh());
			if (App->editor->drawSelectedVertex) DrawVertexNormals(tempComponentMesh->GetMesh());
		}
		else if (gObject->active)
		{
			if (App->editor->drawSelectedFaces)  DrawFaceNormals(tempComponentMesh->GetMesh());
			if (App->editor->drawSelectedVertex) DrawVertexNormals(tempComponentMesh->GetMesh());
		}
	}
}

void  ModuleRenderer3D::ImportCube()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cube.fbx");
}

void  ModuleRenderer3D::ImportSphere()
{
	App->importer->ReadFile("Assets/Models/Primitives/Sphere.fbx");
}

void  ModuleRenderer3D::ImportCone()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cone.fbx");
}

void  ModuleRenderer3D::ImportCylinder()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cylinder.fbx");
}

void  ModuleRenderer3D::ImportTorus()
{
	App->importer->ReadFile("Assets/Models/Primitives/Torus.fbx");
}