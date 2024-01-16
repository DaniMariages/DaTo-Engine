#include "ModuleScene.h"
#include "Globals.h"
#include "ModuleCamera3D.h"
#include "ModuleImport.h"
#include "ModuleWindow.h"
#include "ModuleFont.h"
#include "ComponentCanvas.h"

#include <map>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	rootGameObject = CreateGameObject("Scene", nullptr);
	gameCameraObject = CreateGameObject("MainCamera", rootGameObject);

	ImGuizmo::Enable(true);

	Demo = true;

	gameTime.Stop();
	gameTime.ReStart();
}

// Destructor
ModuleScene::~ModuleScene()
{}

// Called before render is available
bool ModuleScene::Init()
{
	LOG("Creating Module Scene");
	bool ret = true;

	App->fonts->actualFont = App->fonts->FontLoader(120, "./Fonts/Elianto.otf");

	gameCamera = new ComponentCamera(gameCameraObject);
	gameCameraObject->AddComponent(gameCamera);

	//Set the game camera starter position
	gameCameraObject->transform->SetPosition(float3(0.0f, 5.0f, -20.0f));
	gameCameraObject->transform->SetRotation(Quat::FromEulerXYZ(0.0f, 0.0, 0.0f));

	totalCameras.push_back(gameCamera);
	totalCameras.push_back(App->camera->editorCamera);

	gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	gizmoMode = ImGuizmo::MODE::WORLD;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleScene::Update(float dt)
{
	if (Demo) 
	{
		DemoScene();
		Demo = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	else if (App->input->GetKey(SDL_SCANCODE_E) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	else if (App->input->GetKey(SDL_SCANCODE_R) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::SCALE;
	else if (App->input->GetKey(SDL_SCANCODE_T) && !ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
		gizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModuleScene::CleanUp()
{
	LOG("Destroying Module Scene");
	return true;
}

GameObject* ModuleScene::CreateGameObject(std::string Name, GameObject* parent)
{
	GameObject* newGameObject = new GameObject(Name, parent);
	if (parent != nullptr) parent->AddChildren(newGameObject);
	gameObjects.push_back(newGameObject);

	return newGameObject;
}

void ModuleScene::DeleteGameObject(GameObject* gameObject)
{
	//If the game object has children delete them first
	if (!gameObject->children.empty())
	{
		for (unsigned int i = 0; i < gameObjects.size(); ++i)
		{
			for (unsigned int j = 0; j < gameObject->children.size(); ++j)
			{
				//Delete the childrens from game objects list
				if (gameObjects[i] == gameObject->children[j])
					gameObjects.erase(gameObjects.begin() + i);
			}
		}

		//Delete all the children
		do
		{
			int i = 0;
			gameObject->children.erase(gameObject->children.begin() + i);
			i++;

		} while (!gameObject->children.empty());
	}

	//Delete of children parent list
	for (unsigned int i = 0; i < gameObject->Parent->children.size(); ++i)
	{
		if (gameObject == gameObject->Parent->children[i])
		{
			gameObject->Parent->children.erase(gameObject->Parent->children.begin() + i);
			break;
		}
	}

	//When references deleted, delete the game Object
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObject == gameObjects[i])
		{
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}

	//Set the gameObjects null
	gameObject = nullptr;
	gameObjectSelected = nullptr;
}

void ModuleScene::SelectGameObject(const LineSegment& ray)
{
	//Line Segment to debug
	pickingDebug = ray;

	std::map<float, ComponentMesh*> cMeshCandidates;
	for (const auto& gameObject : gameObjects) 
	{
		for (const auto& component : gameObject->components) 
		{
			if (component->type == typeComponent::Mesh) 
			{
				ComponentMesh* tempMesh = static_cast<ComponentMesh*>(gameObject->GetComponent(typeComponent::Mesh));
				float closest;
				float furthest;

				if (ray.Intersects(tempMesh->gAABB, closest, furthest))
				{
					//Sky box shouldn't can be selected
					if (!InsideBBObject(ray.a, tempMesh->gAABB))
						cMeshCandidates[closest] = tempMesh;
				}
			}
		}
	}

	std::vector<ComponentMesh*> cMeshSorted;
	for (auto& candidate : cMeshCandidates) 
	{
		cMeshSorted.push_back(candidate.second);
	}

	for (ComponentMesh* mesh : cMeshSorted) 
	{
		if (mesh != nullptr && mesh->parent != nullptr) 
		{
			mesh->parent->selected = false;
		}
	}

	for (ComponentMesh* mesh : cMeshSorted) 
	{
		if (mesh != nullptr) 
		{
			LineSegment localRay = ray;

			ComponentTransform* tempTrans = (ComponentTransform*)mesh->parent->GetComponent(typeComponent::Transform);
			localRay.Transform(tempTrans->GetGlobalTransform().Inverted());

			// Iterate over triangles in the mesh.
			for (unsigned int i = 0; i < mesh->GetMesh()->indices.size(); i += 3) 
			{
				uint triangle_indices[3] = { mesh->GetMesh()->indices[i], mesh->GetMesh()->indices[i + 1], mesh->GetMesh()->indices[i + 2] };

				float3 aPoint(mesh->GetMesh()->vertices[triangle_indices[0]].Position);
				float3 bPoint(mesh->GetMesh()->vertices[triangle_indices[1]].Position);
				float3 cPoint(mesh->GetMesh()->vertices[triangle_indices[2]].Position);

				Triangle triangle(aPoint, bPoint, cPoint);

				// Check if ray intersect with the triangle
				if (localRay.Intersects(triangle, nullptr, nullptr))
				{
					// If mesh parent is not null and is selectable with mouse picking
					if (mesh->parent != nullptr && mesh->parent->selectableWithMP)
					{
						// Game object selected will be mesh parent
						gameObjectSelected = mesh->parent;
						mesh->parent->selected = true;

						// Iterate through all game objects in the scene.
						for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it) 
						{
							// Unselect other game objects.
							if ((*it) != mesh->parent) 
							{
								(*it)->selected = false;
							}
						}
					}

					//Return when the ray interesect
					return;
				}
			}
		}
	}

	// No intersection found, clear the selection for all meshes.
	for (auto it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it) 
	{
		(*it)->selected = false;
	}
}

void ModuleScene::DrawImGuizmo(ImVec2 windowPos, ImVec2 contentRegionMax, float offset)
{
	if (App->scene->gameObjectSelected != nullptr && App->scene->gameObjectSelected != App->scene->rootGameObject)
	{
		ImGuizmo::BeginFrame();

		ComponentTransform* selected_transform = (ComponentTransform*)App->scene->gameObjectSelected->GetComponent(typeComponent::Transform);

		float4x4 viewMatrix = App->camera->editorCamera->frustum.ViewMatrix();
		viewMatrix.Transpose();

		float4x4 projectionMatrix = App->camera->editorCamera->frustum.ProjectionMatrix();
		projectionMatrix.Transpose();

		float4x4 modelProjection = selected_transform->GetLocalTransform();
		modelProjection.Transpose();

		ImGuizmo::SetRect(windowPos.x, windowPos.y + offset, contentRegionMax.x, contentRegionMax.y);
		ImGuizmo::SetDrawlist();

		float modelPtr[16];
		memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));

		ImGuizmo::MODE finalMode = (gizmoOperation == ImGuizmo::OPERATION::SCALE ? ImGuizmo::MODE::LOCAL : gizmoMode);
		ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), gizmoOperation, finalMode, modelPtr);

		if (ImGuizmo::IsUsing())
		{
			float4x4 newMatrix;
			newMatrix.Set(modelPtr);
			modelProjection = newMatrix.Transposed();

			selected_transform->SetLocalTransform(modelProjection);
			App->scene->gameObjectSelected->transform->UpdateTransform();
		}
	}
}

bool ModuleScene::InsideBBObject(const float3& point, AABB& aabb)
{
	return point.x >= aabb.minPoint.x && point.x <= aabb.maxPoint.x
		&& point.y >= aabb.minPoint.y && point.y <= aabb.maxPoint.y
		&& point.z >= aabb.minPoint.z && point.z <= aabb.maxPoint.z;
}

void ModuleScene::DemoScene()
{
	ComponentUI* compUI = new ComponentUI(UI_Type::DEF, App->scene->rootGameObject, 0, 0, 0, 0, nullptr);
	ComponentTransform* transform;

	compUI->CreateGameObjectUI(App->scene->rootGameObject, UI_Type::CANV, App->editor->GetWindowSize().x, App->editor->GetWindowSize().y, 200, 200, nullptr, nullptr);

	ComponentCanvas* canvUI = new ComponentCanvas(App->scene->canvas, App->editor->GetWindowSize().x, App->editor->GetWindowSize().y, 0, 0);

	compUI->CreateGameObjectUI(App->scene->canvas, UI_Type::TEXT, 25, 25, 0, 0, nullptr, "START", 1, nullptr, (canvUI->widthPanel / 2) - (canvUI->widthPanel / 8), (canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 9), 25, 25);
	transform = dynamic_cast<ComponentTransform*>(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->GetComponent(typeComponent::Transform));
	transform->SetPosition(float3((float)((canvUI->widthPanel / 2) - (canvUI->widthPanel / 8)), (float)((canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 9)), 0));

	compUI->CreateGameObjectUI(App->scene->canvas, UI_Type::INPUT_TEXT, 20, 20, 0, 0, "Assets/Textures/Input_Text_Empty.png", "", 1, nullptr, (canvUI->widthPanel / 2) - (canvUI->widthPanel / 10), (canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 3.6), 160, 80);
	transform = dynamic_cast<ComponentTransform*>(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->GetComponent(typeComponent::Transform));
	transform->SetPosition(float3((float)((canvUI->widthPanel / 2) - (canvUI->widthPanel / 10)), (float)((canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 3.6)), 0));

	compUI->CreateGameObjectUI(App->scene->canvas, UI_Type::BUTTON, 1, 1, 0, 0, "Assets/Textures/Button3.png", nullptr, 0, nullptr, (canvUI->widthPanel / 2) - (canvUI->widthPanel / 6), (canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 20), 160, 80);
	transform = dynamic_cast<ComponentTransform*>(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->GetComponent(typeComponent::Transform));
	transform->SetPosition(float3(2, 3, 0));

	compUI->CreateGameObjectUI(App->scene->canvas, UI_Type::IMAGE, 1, 1, 0, 0, "Assets/Textures/Unchecked-checkbox.png", nullptr, 0, nullptr, (canvUI->widthPanel / 2) - (canvUI->widthPanel / 6), (canvUI->heigthPanel / 2) + (canvUI->heigthPanel / 20), 1, 1);
	transform = dynamic_cast<ComponentTransform*>(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->GetComponent(typeComponent::Transform));
	transform->SetPosition(float3(2, 3, 0));

	compUI->CreateGameObjectUI(App->scene->canvas, UI_Type::IMAGE, (uint)canvUI->widthPanel, (uint)canvUI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png", nullptr, 1, nullptr, 0, 0, (uint)canvUI->widthPanel, (uint)canvUI->heigthPanel);
	transform = dynamic_cast<ComponentTransform*>(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->GetComponent(typeComponent::Transform));
	transform->SetPosition(float3(App->editor->GetWindowSize().x, App->editor->GetWindowSize().y, 0));
}
