#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ComponentCanvas.h"

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

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled), context()
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	ilInit();
	ILenum ILerror = ilGetError();

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
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
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
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

	UseCheckerTexture();

	Grid.axis = true;

	//BakerHouse
	App->importer->ReadFile("Assets/Models/BakerHouse.fbx");
	App->importer->ReadFile("Assets/Textures/BakerHouse.png");

	//Street Environment
	//App->importer->ReadFile("Assets/Models/StreetEnvironment/StreetEnvironment.fbx");

	//Skybox
	App->importer->ReadFile("Assets/Models/Skybox.fbx");
	App->importer->ReadFile("Assets/Textures/Skybox.png");

	OnResize(App->window->width, App->window->height);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->editorCamera->GetViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->editorCamera->GetPos().x, App->camera->editorCamera->GetPos().y, App->camera->editorCamera->GetPos().z);

	for (uint i = 0; i < MAX_LIGHTS; ++i) lights[i].Render();

	for (uint n = 0; n < App->scene->gameObjects.size(); n++)
		App->scene->gameObjects[n]->Update();

	for (unsigned int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		ComponentTexture* compTex = (ComponentTexture*)App->scene->gameObjects[i]->GetComponent(typeComponent::Material);
		ComponentCanvas* compCanv = (ComponentCanvas*)App->scene->gameObjects[i]->GetComponent(typeComponent::Canvas);
		ComponentUI* compUI = (ComponentUI*)App->scene->gameObjects[i]->GetComponent(typeComponent::UI);

		if (compCanv != nullptr)
		{
			RenderCanvas(App->scene->gameObjects[i], compCanv);
		}

		if (compCanv != nullptr)
		{
			RenderUI(App->scene->gameObjects[i], compUI, false, compTex);
		}
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Draw Editor Camera
	App->camera->editorCamera->RenderBuffers(true);
	App->camera->editorCamera->Update();

	//Render the Bounding Box for all meshes
	RenderBB();

	//Render ray of editor camera
	RenderRay();

	//Draw Grid
	Grid.Render();

	//Draw Frustum Box
	if (App->scene->gameCameraObject->active)
		App->scene->gameCamera->DrawFrustumCube();

	//Draw meshes
	IterateDrawMesh();

	App->camera->editorCamera->RenderBuffers(false);

	//-------------------------------

	//Draw Game Camera
	App->scene->gameCamera->RenderBuffers(true);
	App->scene->gameCamera->Update();

	if (App->scene->gameCameraObject->active)
	{
		//Draw the models in Game Camera
		IterateDrawMesh();

		//Draw the UI in Game Camera
		for (int i = 1; i < App->scene->gameObjects.size(); i++)
		{
			if (App->scene->gameObjects[i]->active)
			{
				ComponentTexture* objectTexture = dynamic_cast<ComponentTexture*>(App->scene->gameObjects[i]->GetComponent(typeComponent::Material));
				ComponentUI* objectUI = dynamic_cast<ComponentUI*>(App->scene->gameObjects[i]->GetComponent(typeComponent::UI));
				ComponentCanvas* canvas_UI = dynamic_cast<ComponentCanvas*>(App->scene->gameObjects[i]->GetComponent(typeComponent::Canvas));

				/*if (App->scene->pause != nullptr)
				{
					for (int j = 0; j < app->scene->pause->childrens.size(); j++)
					{
						if (App->scene->AllGameObjectManagers[i] == app->scene->pause->childrens[j])
						{
							app->scene->pause->childrens[j]->isActive = app->scene->openPauseMenu;

							if (objectTexture != nullptr)
							{
								objectTexture->colorTexture.a = 0.5;
							}
						}
					}
				}*/

				if (canvas_UI != nullptr)
				{
					RenderCanvas(App->scene->gameObjects[i], canvas_UI);
				}
				else
				{
					if (objectUI != nullptr)
					{
						RenderUI(App->scene->gameObjects[i], objectUI, true, objectTexture);
					}
				}
			}
		}
	}

	App->scene->gameCamera->RenderBuffers(false);

	//Draw Editor
	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	App->camera->editorCamera->UnloadBuffers();
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	for (unsigned int i = 0; i < App->scene->totalCameras.size(); i++)
	{
		App->scene->totalCameras[i]->LoadBuffers(width, height);
	}
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::IterateDrawMesh()
{
	for (unsigned int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (App->scene->gameObjects[i]->HasComponent(typeComponent::Mesh))
		{
			std::vector<Component*> meshComponents = App->scene->gameObjects[i]->GetComponents(typeComponent::Mesh);
			std::vector<Component*>::iterator item = meshComponents.begin();
			for (; item != meshComponents.end(); ++item)
			{
				ComponentTexture* componentTex = (ComponentTexture*)App->scene->gameObjects[i]->GetComponent(typeComponent::Material);
				ComponentMesh* tempComponentMesh = (ComponentMesh*)(*item);
				ComponentTransform* tempTrans = (ComponentTransform*)App->scene->gameObjects[i]->GetComponent(typeComponent::Transform);
				if (componentTex != nullptr && App->scene->gameObjects[i]->active && App->scene->gameObjects[i]->drawTexture && InsideCamera(App->scene->gameCamera, tempComponentMesh->gAABB))
				{
					DrawMesh(tempComponentMesh->GetMesh(), tempTrans->GetGlobalTransform(), componentTex->GetTexture()->textID);

					if (App->editor->drawAllFaces == true)
						DrawFaceNormals(tempComponentMesh->GetMesh(), tempTrans->GetPosition(), tempTrans->GetScale(), tempTrans->GetRotation());

					if (App->editor->drawAllVertex == true)
						DrawVertexNormals(tempComponentMesh->GetMesh(), tempTrans->GetPosition(), tempTrans->GetScale(), tempTrans->GetRotation());
				}
				else if (App->scene->gameObjects[i]->active && InsideCamera(App->scene->gameCamera, tempComponentMesh->gAABB))
				{
					DrawMesh(tempComponentMesh->GetMesh(), tempTrans->GetGlobalTransform());

					if (App->editor->drawAllFaces == true)
						DrawFaceNormals(tempComponentMesh->GetMesh(), tempTrans->GetPosition(), tempTrans->GetScale(), tempTrans->GetRotation());

					if (App->editor->drawAllVertex == true)
						DrawVertexNormals(tempComponentMesh->GetMesh(), tempTrans->GetPosition(), tempTrans->GetScale(), tempTrans->GetRotation());
				}

				if (App->editor->drawSelectedFaces) DrawSelectedNormals();
				if (App->editor->drawSelectedVertex) DrawSelectedNormals();
			}
		}
	}
}

void ModuleRenderer3D::DrawMesh(mesh* mesh, float4x4 transform, uint id)
{
	// Update the transfor for meshes
	glPushMatrix();
	glMultMatrixf(transform.Transposed().ptr());

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

	//Disable the actual transform
	glPopMatrix();
}

void ModuleRenderer3D::DrawFaceNormals(mesh* Mesh, float3 position, float3 scale, Quat rotation)
{
	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < Mesh->indices.size(); i += 3)
	{
		const unsigned int idx1 = Mesh->indices[i];
		const unsigned int idx2 = Mesh->indices[i + 1];
		const unsigned int idx3 = Mesh->indices[i + 2];

		float3 vertex1 = Mesh->vertices[idx1].Position.Mul(scale);
		float3 vertex2 = Mesh->vertices[idx2].Position.Mul(scale);
		float3 vertex3 = Mesh->vertices[idx3].Position.Mul(scale);

		// Apply rotation
		vertex1 = rotation * vertex1;
		vertex2 = rotation * vertex2;
		vertex3 = rotation * vertex3;

		// Translate to global position
		vertex1 += position;
		vertex2 += position;
		vertex3 += position;

		// Calculate the face normal
		float3 faceNormal = CalculateFaceNormal(vertex1, vertex2, vertex3);

		// Calculate the face center
		float3 centroid = (vertex1 + vertex2 + vertex3) / 3.0f;

		// Draw the face normal line
		float length = 0.5f; // Lines length
		glVertex3f(centroid.x, centroid.y, centroid.z);
		glVertex3f(centroid.x + length * faceNormal.x, centroid.y + length * faceNormal.y, centroid.z + length * faceNormal.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}

float3 ModuleRenderer3D::CalculateFaceNormal(const float3& vertex1, const float3& vertex2, const float3& vertex3)
{
	float3 edge1 = vertex2 - vertex1;
	float3 edge2 = vertex3 - vertex1;
	return Cross(edge1, edge2).Normalized();
}

void ModuleRenderer3D::DrawVertexNormals(mesh* Mesh, float3 position, float3 scale, Quat rotation)
{
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);

	for (unsigned int j = 0; j < Mesh->vertices.size(); ++j)
	{
		float3 vertex = Mesh->vertices[j].Position.Mul(scale);

		// Apply rotation
		vertex = rotation * vertex;

		// Translate to global position
		vertex += position;

		float3 normal = Mesh->vertices[j].Normal;

		// Draw the vertex normal line
		float length = 0.5f; // Lines length
		glVertex3f(vertex.x, vertex.y, vertex.z);
		glVertex3f(vertex.x + length * normal.x, vertex.y + length * normal.y, vertex.z + length * normal.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}

void ModuleRenderer3D::DrawSelectedNormals()
{
	if (App->editor->drawSelectedFaces || App->editor->drawSelectedVertex)
	{
		GameObject* selectedObject = App->scene->gameObjectSelected;

		if (selectedObject)
		{
			ComponentTransform* transform = static_cast<ComponentTransform*>(selectedObject->GetComponent(typeComponent::Transform));

			for (unsigned int i = 0; i < selectedObject->components.size(); ++i)
			{
				if (selectedObject->components[i]->type == typeComponent::Mesh)
				{
					ComponentMesh* compMesh = static_cast<ComponentMesh*>(selectedObject->components[i]);
					mesh* Mesh = compMesh->GetMesh();

					if (App->editor->drawSelectedFaces)
					{
						DrawFaceNormals(Mesh, transform->GetPosition(), transform->GetScale(), transform->GetRotation());
					}

					if (App->editor->drawSelectedVertex)
					{
						DrawVertexNormals(Mesh, transform->GetPosition(), transform->GetScale(), transform->GetRotation());
					}
				}
			}
		}
	}
}

void ModuleRenderer3D::DrawBox(float3* vertices, float3 color)
{
	uint indices[24] = {

		0,2,2,
		6,6,4,
		4,0,0,
		1,1,3,
		3,2,4,
		5,6,7,
		5,7,3,
		7,1,5

	};

	glLineWidth(3.0f); //Makes the lines a little thicker for visual reasons.
	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); i++) 
	{
		glVertex3fv(vertices[indices[i]].ptr());
	}

	glColor3f(255.f, 255.f, 255.f);
	glLineWidth(1.0f);
	glEnd();
}

void ModuleRenderer3D::RenderBB()
{
	//Get all the game objects
	for (unsigned int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		//Get all the components from the Game Object
		for (unsigned int j = 0; j < App->scene->gameObjects[i]->components.size(); j++)
		{
			//If has a Mesh component, then update its bounding box
			if (App->scene->gameObjects[i]->HasComponent(typeComponent::Mesh))
			{
				ComponentMesh* compMesh = (ComponentMesh*)App->scene->gameObjects[i]->GetComponent(typeComponent::Mesh);

				//Update the position of the Bounding Box
				compMesh->UpdateBoundingBoxes();

				//If Bounding Box is inside camera, then draw it
				if (InsideCamera(App->scene->gameCamera, compMesh->gAABB) && compMesh->showBB && App->editor->drawAllBoxes) 
					compMesh->RenderBoundingBoxes();
			}
		}
	}
}

void ModuleRenderer3D::RenderRay()
{
	glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(&App->scene->pickingDebug.a.x);
	glVertex3fv(&App->scene->pickingDebug.b.x);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();
}

bool ModuleRenderer3D::InsideCamera(const ComponentCamera* camera, const AABB& aabb)
{
	//If frustum culling is enabled then continue
	if (camera->frustumCulling)
	{
		Plane frustumPlanes[6];
		camera->frustum.GetPlanes(frustumPlanes);

		float3 cornerPoints[8];
		aabb.GetCornerPoints(cornerPoints);

		for (int i = 0; i < 6; ++i)
		{
			uint pointsInside = 8;
			for (int j = 0; j < 8; ++j)
			{
				if (frustumPlanes[i].IsOnPositiveSide(cornerPoints[j])) --pointsInside;
			}

			if (pointsInside == 0) return false;
		}
	}
	return true;
}

void ModuleRenderer3D::RenderCanvas(GameObject* canvasObject, ComponentCanvas* canvas)
{
	if (App->scene->canvas != nullptr)
	{
		ComponentTransform* compTrans = (ComponentTransform*)canvasObject->GetComponent(typeComponent::Transform);
		ComponentCanvas* compCanvas = (ComponentCanvas*)App->scene->canvas->GetComponent(typeComponent::Canvas);

		compCanvas->Draw();
		compTrans->UpdateTransform();
	}
}

void ModuleRenderer3D::RenderUI(GameObject* parent, ComponentUI* uiElement, bool isGameMode, ComponentTexture* texture)
{
	UIPlane* ui_Plane = nullptr;

	ComponentTransform* transform = dynamic_cast<ComponentTransform*>(parent->GetComponent(typeComponent::Transform));

	//Put plane cases! if game or editor

	if (!isGameMode)
	{
		ui_Plane = uiElement->PlaneInScene;
		glPushMatrix();
		glMultMatrixf((float*)parent->transform->GetGlobalTransform().Transposed().ptr());
	}

	if (isGameMode)
	{
		ui_Plane = uiElement->PlaneInScene;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, App->editor->GameWindowSize.x, App->editor->GameWindowSize.y, 0.0, 0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		glMultMatrixf((float*)parent->transform->GetGlobalTransform().Transposed().ptr());
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(uiElement->color[0], uiElement->color[1], uiElement->color[2], uiElement->color[3]);

	if (texture != nullptr)
	{
		glColor4f(texture->texColor.r, texture->texColor.g, texture->texColor.b, texture->texColor.a);
	}

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (uiElement->textCH == "")
	{
		if (texture != nullptr)
		{
			if (texture->GetTexture())
			{
				if (texture->GetTexture()->textID != 0)
				{
					if (texture->GetTexture()->textID != 0)
					{
						glBindTexture(GL_TEXTURE_2D, texture->GetTexture()->textID);
					}
					else if (uiElement->texture != nullptr)
					{
						if (uiElement->texture->textID != 0)
						{
							glBindTexture(GL_TEXTURE_2D, uiElement->texture->textID);
						}
					}
				}
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, ui_Plane->buffer[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, ui_Plane->buffer[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_Plane->buffer[1]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		//ComponentText* text = UI_Element->textComp;
		const char* letters = uiElement->textCH.c_str();

		std::vector<Component*> objectPanels = parent->GetComponents(typeComponent::Mesh);

		for (int j = 0; j < objectPanels.size(); j++)
		{
			if (uiElement->font->AllCharacters.at(letters[j]).textID != 0)
			{
				auto test = uiElement->font->AllCharacters.at(letters[j]);
				glBindTexture(GL_TEXTURE_2D, test.textID);
			}

			ComponentMesh* panel = dynamic_cast<ComponentMesh*>(objectPanels.at(j));

			if (panel != nullptr)
			{
				if (panel->active)
				{
					if (panel->GetMesh() != nullptr)
					{
						glBindBuffer(GL_ARRAY_BUFFER, panel->GetMesh()->VBO);
						glVertexPointer(3, GL_FLOAT, 0, NULL);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glBindBuffer(GL_ARRAY_BUFFER, panel->GetMesh()->VN);
						glTexCoordPointer(2, GL_FLOAT, 0, NULL);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, panel->GetMesh()->EBO);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
					}
				}
			}
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//if (!isGameMode)
	glPopMatrix();

	transform->UpdateTransform();
}

void ModuleRenderer3D::ImportCube()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cube.fbx");
}

void ModuleRenderer3D::ImportSphere()
{
	App->importer->ReadFile("Assets/Models/Primitives/Sphere.fbx");
}

void ModuleRenderer3D::ImportCone()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cone.fbx");
}

void ModuleRenderer3D::ImportCylinder()
{
	App->importer->ReadFile("Assets/Models/Primitives/Cylinder.fbx");
}

void ModuleRenderer3D::ImportTorus()
{
	App->importer->ReadFile("Assets/Models/Primitives/Torus.fbx");
}