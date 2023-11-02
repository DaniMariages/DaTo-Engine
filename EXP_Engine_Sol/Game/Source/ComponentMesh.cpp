#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "../External/ImGui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
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
