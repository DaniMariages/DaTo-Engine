#pragma once
#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

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

	GameObject* CreateGameObject(std::string Name, GameObject* parent);
	void DeleteGameObject(GameObject* gameObject);

	// Mouse picking function
	void SelectGameObject(const LineSegment& ray);
	LineSegment pickingDebug;

	//Guizmo
	void DrawImGuizmo();
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::WORLD;

public: 

	GameObject* rootGameObject = nullptr;
	std::vector<GameObject*> gameObjects;
	GameObject* gameObjectSelected = nullptr;

	GameObject* gameCameraObject = nullptr;
	ComponentCamera* gameCamera = nullptr;
	std::vector<ComponentCamera*> totalCameras;
};