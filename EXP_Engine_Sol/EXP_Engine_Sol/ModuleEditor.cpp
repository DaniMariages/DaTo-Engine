#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGuI/backends/imgui_impl_sdl2.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor ()
{
}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;
	ImGui::CreateContext();

	return ret;
}

void ModuleEditor :: DrawEditor()
{
	//Start the Dear InGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Text("Hello world!");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Configuratiobn"))
	{
		ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size());
	}

	ImGui::ShowDemoWindow();

	ImGui :: Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui :: GetDrawData());
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	return true;
}