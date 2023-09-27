#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGuI/backends/imgui_impl_sdl2.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

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
    //// Setup Dear ImGui context
    //// Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //// Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ////ImGui::StyleColorsClassic();

    //// Setup Platform/Renderer backends
    //ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
    //ImGui_ImplOpenGL3_Init("#version 130");
	return ret;
}

void ModuleEditor :: DrawEditor()
{

	////Start the Dear ImGui frame
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame();
	//ImGui::NewFrame();

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if(ImGui::BeginMainMenuBar()) {
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		ImGui::Text("Hello world!");
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();
	//}

	//ImGuiIO& io = ImGui::GetIO(); (void)io;


	//// Rendering
	//ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	////glClearColor(1.0, 1.0, 1.0, 0.0);
	////glClear(GL_COLOR_BUFFER_BIT);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//App->renderer3D->Grid.Render();
	//SDL_GL_SwapWindow(App->window->window);
 
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	//// Cleanup
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();

	//SDL_GL_DeleteContext(context);
	return true;
}