#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "Component.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "ModuleCamera3D.h"

#include "../External/Glew/include/glew.h"
#include "../External/SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Game/External/Glew/libx86/glew32.lib")

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;
	App->mFPSLog.reserve(30);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		IoConfirmer = true;
	}

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	return ret;
}

void ModuleEditor::DrawEditor()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
	ImGuizmo::Enable(true);
	ImGuizmo::BeginFrame();

	aFPS = App->FPS();

	UpdateFPS(aFPS);
	MainMenuBar();

	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	if (show_metrics_window) ImGui::ShowMetricsWindow(&show_metrics_window);

	if (show_hierarchy_window) DrawHierarchy();
	if (show_inspector_window) DrawInspector();
	if (show_delete_scene_modal) DrawSceneAlert();

	if (ImGui::Begin("Scene"), true)
	{
		//Render the screen Scene (Editor camera)
		ImVec2 size = ImGui::GetContentRegionAvail();
		App->camera->editorCamera->SetAspectRatio(size.x / size.y);
		ImGui::Image((ImTextureID)App->camera->editorCamera->TCB, size, ImVec2(0, 1), ImVec2(1, 0));

		//Info needed for ImGuizmo
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 contentRegionMax = ImGui::GetContentRegionMax();
		int offset = ImGui::GetFrameHeight() / 2.0f;

		App->scene->DrawImGuizmo(windowPos, contentRegionMax, offset);

		//Scene info needed for Mouse Picking function
		ImVec2 sizeScene = ImGui::GetContentRegionAvail();
		ImVec2 mousePosition = ImGui::GetMousePos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		//This function is here because we need the info of the screen
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			MousePicking(mousePosition, windowPos, windowSize, offset);

		ImGui::End();
	}

	if (ImGui::Begin("Game"), true)
	{
		//Render the screen Game (Game camera)
		ImVec2 size = ImGui::GetContentRegionAvail();
		App->scene->gameCamera->SetAspectRatio(size.x / size.y);
		ImGui::Image((ImTextureID)App->scene->gameCamera->TCB, size, ImVec2(0, 1), ImVec2(1, 0));

		GameWindowSize = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
		GameWindowPos = ImGui::GetWindowPos();

		if (App->scene->gameCamera != nullptr)
		{
			mousePosInViewport.x = App->input->GetMouseX() - ImGui::GetCursorScreenPos().x;
			mousePosInViewport.y = App->input->GetMouseY() - ImGui::GetCursorScreenPos().y;

		}

		ImGui::End();
	}

	Config();
	Console();
	DrawPausePlay();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Render();

	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	return true;
}

void ModuleEditor::UpdateFPS(const float aFPS)
{
	if (App->mFPSLog.size() < 30)
	{
		App->mFPSLog.push_back(aFPS);
	}
	else
	{
		for (unsigned int i = 0; i < App->mFPSLog.size(); i++)
		{
			if (i + 1 < App->mFPSLog.size())
			{
				float copy = App->mFPSLog[i + 1];
				App->mFPSLog[i] = copy;
			}
		}
		App->mFPSLog[App->mFPSLog.capacity() - 1] = aFPS;
	}
}

void ModuleEditor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit")) {

				App->input->quit = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Game Objects"))
		{
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube")) {
					App->renderer3D->ImportCube();
				}
				if (ImGui::MenuItem("Sphere")) {
					App->renderer3D->ImportSphere();
				}
				if (ImGui::MenuItem("Cone")) {
					App->renderer3D->ImportCone();
				}
				if (ImGui::MenuItem("Torus")) {
					App->renderer3D->ImportTorus();
				}
				if (ImGui::MenuItem("Cylinder")) {
					App->renderer3D->ImportCylinder();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Empty Game Object"))
			{
				App->scene->CreateGameObject("Empty Game Object", App->scene->rootGameObject);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Configuration")) {
				show_config_window = !show_config_window;
			}
			if (ImGui::MenuItem("Demo Window")) {
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Metrics Window")) {
				show_metrics_window = !show_metrics_window;
			}
			if (ImGui::MenuItem("Hierarchy")) {
				show_hierarchy_window = !show_hierarchy_window;
			}
			if (ImGui::MenuItem("Inspector")) {
				show_inspector_window = !show_inspector_window;
			}
			if (ImGui::MenuItem("Console")) {
				show_console = !show_console;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			ImGui::Text("DaTo Engine by Dani Mariages & Toni Romanos");
			sprintf(label, "Github Repository");
			if (ImGui::Selectable(label, true))	RequestBrowser("https://github.com/DaniMariages/DaTo-Engine");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);
}

void ModuleEditor::Config() {

	ImGuiStyle& style = ImGui::GetStyle();

	if (show_config_window)
	{
		ImGui::Begin("Configuration", &show_config_window, ImGuiWindowFlags_DockNodeHost);
		if (ImGui::CollapsingHeader("Display"))
		{
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
				App->window->FullScreen(fullscreen);
			}
			if (ImGui::Checkbox("Borderless", &borderless)) {
				App->window->Borderless(borderless);
			}
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			//Display current hardware and driver capabilities

			ImGui::Text("Using Glew:");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glewGetString(GLEW_VERSION));

			ImGui::Text("Vendor: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_VENDOR));

			ImGui::Text("Renderer: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_RENDERER));

			ImGui::Text("OpenGL version supported: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_VERSION));

			ImGui::Text("GLSL: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		}

		if (ImGui::CollapsingHeader("Framerate"))
		{
			ImGui::PlotHistogram("##FPS", &App->mFPSLog[0], App->mFPSLog.size(), 0, "", 0.0f, 100.0f, ImVec2(300, 100));
		}

		if (ImGui::CollapsingHeader("Draw Settings"))
		{
			if (ImGui::Checkbox("Draw", &drawAll)) {}
			if (ImGui::Checkbox("Normals", &drawAllFaces)) {}
			if (ImGui::Checkbox("Vertex", &drawAllVertex)) {}
			if (ImGui::Checkbox("Textures", &drawTextures)) {}
			if (ImGui::Checkbox("Bounding Boxes", &drawAllBoxes)) {}
		}

		if (ImGui::CollapsingHeader("View"))
		{
			if (ImGui::Button("Top")) { App->camera->TopView(); }
			ImGui::SameLine();
			if (ImGui::Button("Front")) { App->camera->FrontView(); }
			ImGui::SameLine();
			if (ImGui::Button("Left side")) { App->camera->Side_1View(); }
			ImGui::SameLine();
			if (ImGui::Button("Right side")) { App->camera->Side_2View(); }
			ImGui::SameLine();
			if (ImGui::Button("Back")) { App->camera->BackView(); }
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			if (ImGui::Checkbox("Shader", &shader))
			{
				if (shader) glEnable(GL_LIGHTING);
				else glDisable(GL_LIGHTING);
			}

			if (ImGui::Checkbox("Cull face", &cullface))
			{
				if (cullface) glEnable(GL_CULL_FACE);
				else glDisable(GL_CULL_FACE);
			}
		}
		ImGui::End();
	}
}

void ModuleEditor::Console()
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (show_console)
	{
		ImGui::Begin("Console", &show_console);

		for (int i = 0; i < log_history.size(); i++)
		{
			ImGui::Text("%s", log_history[i].c_str());
		}
		ImGui::End();
	}
}

void ModuleEditor::DrawHierarchy()
{
	if (ImGui::Begin("Hierarchy", &show_hierarchy_window), true)
	{
		HierarchyWindow(App->scene->rootGameObject);
		ImGui::End();
	}
}

void ModuleEditor::DrawInspector()
{
	if (ImGui::Begin("Inspector", &show_inspector_window), true)
	{
		Inspector(App->scene->gameObjectSelected);
		ImGui::End();
	}
}

void ModuleEditor::HierarchyWindow(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (gameObject->selected ? ImGuiTreeNodeFlags_Selected : 0);

		if (!gameObject->active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.4f));

		bool openTreeNode = ImGui::TreeNodeEx(gameObject->Name.c_str(), tree_flags);

		if (!gameObject->active) ImGui::PopStyleColor();

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) //Selectable with left and right mouse button
		{
			gameObject->selected = true; //Draw the gameObject as selected
			App->scene->gameObjectSelected = gameObject; //Assign the game object selected

			for (std::vector<GameObject*>::iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
			{
				//Deselect the rest of game objects to dont show them as selected
				if ((*it) != gameObject) (*it)->selected = false;
			}
		}

		if (gameObject != App->scene->rootGameObject)
		{
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("Dragged_Object", gameObject, sizeof(GameObject));
				childObject = gameObject;
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Object"))
				{
					gameObject->AddChildren(childObject);
					childObject = nullptr;
				}

				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::BeginPopupContextItem())
		{
			//Show what Game Object is selected
			ImGui::MenuItem(App->scene->gameObjectSelected->Name.c_str(), NULL, false, false);

			//Create a empty child of Game Object selected
			if (ImGui::MenuItem("Create Empty Child"))
			{
				App->scene->CreateGameObject("EmptyGameObject", App->scene->gameObjectSelected);
			}

			//Hide the selected game object MYTODO: Change enable with hide, cause is not the same
			if (ImGui::MenuItem("Hide"))
			{
				if (gameObject->active) gameObject->Disable();
				else gameObject->Enable();

				if (!gameObject->children.empty())
				{
					if (!gameObject->active) gameObject->DisableParent();
					else gameObject->EnableParent();
				}
			}

			//Delete the selected game object
			if (ImGui::MenuItem("Delete"))
			{
				show_delete_scene_modal = true;
			}
			ImGui::EndPopup();
		}
		ImGui::SetItemTooltip("Right-click to fast options");

		if (openTreeNode)
		{
			if (gameObject != nullptr) {

				if (gameObject->children.size())
				{
					for (uint i = 0; i < gameObject->children.size(); i++)
					{
						HierarchyWindow(gameObject->children[i]);
					}
				}
			}
			ImGui::TreePop();
		}
	}
}

void ModuleEditor::Inspector(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.50f);
		char newName[64];
		std::string uniqueName;

		strcpy_s(newName, gameObject->Name.c_str());
		if (ImGui::InputText("Name", (char*)gameObject->Name.c_str(), 64, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			uniqueName = App->importer->GetUniqueName(newName);
			gameObject->ChangeName(uniqueName.c_str());
		}

		ImGui::Separator();

		ImGui::Text("Parent:");
		ImGui::SameLine();
		if (App->scene->gameObjectSelected->Parent != nullptr)
		{
			ImGui::TextColored(GREEN, "%s", App->scene->gameObjectSelected->Parent->Name.c_str());
		}
		else ImGui::Text("No Parent");

		if (ImGui::Button("Delete"))
		{
			show_delete_scene_modal = true;
		}

		ImGui::SameLine();

		ImGui::Checkbox("Mouse picking", &App->scene->gameObjectSelected->selectableWithMP);

		ImGui::Separator();

		if (ImGui::TreeNode("Draw options"))
		{
			if (ImGui::Checkbox("Draw", &drawSelected))
			{
				if (gameObject->active) gameObject->Disable();
				else gameObject->Enable();

				if (!gameObject->children.empty())
				{
					if (!gameObject->active) gameObject->DisableParent();
					else gameObject->EnableParent();
				}
			}
			if (ImGui::Checkbox("Normals", &drawSelectedFaces));
			if (ImGui::Checkbox("Vertex", &drawSelectedVertex));
			if (ImGui::Checkbox("Textures", &drawSelectedTexture))
			{
				if (gameObject->drawTexture) gameObject->DisableTexture();
				else gameObject->EnableTexture();

				if (!gameObject->children.empty())
				{
					if (!gameObject->drawTexture) gameObject->DisableTextureParent();
					else gameObject->EnableTextureParent();
				}
			}
			if (ImGui::Checkbox("Bounding Box", &drawSelectedBoxes))
			{
				if (gameObject->HasComponent(typeComponent::Mesh))
				{
					ComponentMesh* tMesh = (ComponentMesh*)gameObject->GetComponent(typeComponent::Mesh);
					tMesh->ToggleBB();
				}
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Information"))
		{
			ImGui::Text("Name: %s", gameObject->Name.c_str());

			for (uint i = 0; i < gameObject->components.size(); ++i)
			{
				if (gameObject != nullptr)
				{
					gameObject->components[i]->DrawInspector();
				}
			}
			ImGui::TreePop();
		}
	}
}

void ModuleEditor::DrawSceneAlert()
{
	if (App->scene->gameObjectSelected != App->scene->rootGameObject)
	{
		//Deletable object
		if (!dontShowMeAgain)
		{
			ImGui::OpenPopup("ATENTION");
			if (ImGui::BeginPopupModal("ATENTION", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("This action cannot be undone!\nAre you sure?");
				if (ImGui::Checkbox("Dont show me this again", &dontShowAgainPressed));

				ImGui::Separator();

				if (ImGui::Button("DELETE", ImVec2(120, 0)))
				{
					App->scene->DeleteGameObject(App->scene->gameObjectSelected);
					show_delete_scene_modal = false;
					if (dontShowAgainPressed) dontShowMeAgain = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("CANCEL", ImVec2(120, 0)))
				{
					show_delete_scene_modal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		else
		{
			App->scene->DeleteGameObject(App->scene->gameObjectSelected);
			show_delete_scene_modal = false;
		}
	}
	else
	{
		//Non deletable object
		ImGui::OpenPopup("Action not allowed");
		if (ImGui::BeginPopupModal("Action not allowed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("%s can't be deleted", App->scene->gameObjectSelected->Name.c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				show_delete_scene_modal = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

// Function to Mouse Picking MYTODO: GET THIS OUT OF HERE
void ModuleEditor::MousePicking(ImVec2 mousePosition, ImVec2 sceneWindowPos, ImVec2 sceneWindowSize, float sceneFrameHeightOffset)
{
	ImVec2 normalizedPoint = NormalizePoint(sceneWindowPos.x, sceneWindowPos.y + (sceneFrameHeightOffset * 2), sceneWindowSize.x, sceneWindowSize.y - (sceneFrameHeightOffset * 2), mousePosition);

	normalizedPoint.x = (normalizedPoint.x - 0.5f) / 0.5f;
	normalizedPoint.y = -((normalizedPoint.y - 0.5f) / 0.5f);

	if ((normalizedPoint.x >= -1 && normalizedPoint.x <= 1) && (normalizedPoint.y >= -1 && normalizedPoint.y <= 1))
	{
		App->camera->MousePickingRay(normalizedPoint.x, normalizedPoint.y);
	}

}

void ModuleEditor::DrawPausePlay()
{
	if (ImGui::Begin(" ", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::SameLine(200.0f);

		std::string playStop = App->scene->gameTime.IsRunning() ? "Pause" : "Play";
		if (ImGui::Button(playStop.c_str()))
		{
			if (App->scene->gameTime.IsRunning()) App->scene->gameTime.Stop();
			else if (!App->scene->gameTime.IsRunning()) App->scene->gameTime.Resume();
		}
		ImGui::SameLine();

		if (ImGui::Button("Stop"))
		{
			if (App->scene->gameTime.Read() > 0) App->scene->gameTime.ReStart();
		}
		ImGui::SameLine();

		if (ImGui::Button("Step"))
		{

		}

		ImGui::SameLine();

		ImGui::TextColored(GREEN, "Game time: %.2f", (App->scene->gameTime.Read() / 1000.0f));
	}
	ImGui::End();
}

ImVec2 ModuleEditor::NormalizePoint(float x, float y, float w, float h, ImVec2 originalPoint)
{
	ImVec2 normalizedPoint;

	normalizedPoint.x = (originalPoint.x - x) / ((x + w) - x);
	normalizedPoint.y = (originalPoint.y - y) / ((y + h) - y);

	return normalizedPoint;
}
