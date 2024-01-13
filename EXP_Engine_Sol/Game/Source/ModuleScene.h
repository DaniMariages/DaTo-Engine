#pragma once
#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Timer.h"

#include "../External/MathGeoLib/include/Geometry/LineSegment.h"
#include "../External/ImGui/imgui.h"
#include "../External/ImGuizmo/ImGuizmo.h"

#include <string>
#include <vector>

class ComponentCamera;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateUIElement(const char* name, UI_type type, uint width, uint height, ComponentCanvas* canvas, char* str, GameObject* parent = nullptr, float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f }, Quat rotation = Quat::identity);
	GameObject* CreateGameObject(std::string Name, GameObject* parent);
	void DeleteGameObject(GameObject* gameObject);

	// Mouse picking function
	void SelectGameObject(const LineSegment& ray);
	LineSegment pickingDebug;

	//Guizmo
	void DrawImGuizmo(ImVec2 windowPos, ImVec2 contentRegionMax, float offset);
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::WORLD;

	// This function check if a game object is inside another game object
	bool InsideBBObject(const float3& point, AABB& aabb);

public: 

	GameObject* rootGameObject = nullptr;
	std::vector<GameObject*> gameObjects;
	GameObject* gameObjectSelected = nullptr;

	GameObject* gameCameraObject = nullptr;
	ComponentCamera* gameCamera = nullptr;
	std::vector<ComponentCamera*> totalCameras;

	//Timer
	Timer gameTime;
};