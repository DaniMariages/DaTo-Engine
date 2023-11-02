#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "../External/ImGui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	Mesh = nullptr;
	path = "NO PATH";
	type = typeComponent::Mesh;
}

ComponentMesh::~ComponentMesh() {}

void ComponentMesh::Enable() {}
void ComponentMesh::Disable(){}
void ComponentMesh::Update() {}

void ComponentMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Index: %d", Mesh->indices);
		ImGui::Text("Vertices: %d", Mesh->vertices);
	}
}

void ComponentMesh::SetPath(std::string file_path)
{
	this->path = file_path;
}

void ComponentMesh::SetMesh(mesh* Mesh)
{
	this->Mesh = Mesh;
}
