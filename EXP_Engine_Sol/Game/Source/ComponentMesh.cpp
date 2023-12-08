#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "ModuleEditor.h"

#include "../External/ImGui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	Mesh = new mesh();
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
		ImGui::Text("Indices: %d.", Mesh->indices.size());
		ImGui::Text("Vertices: %d.", Mesh->vertices.size());
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

void ComponentMesh::InitBoundingBoxes(mesh* Mesh)
{
	obb.SetNegativeInfinity();
	gAABB.SetNegativeInfinity();

	std::vector<float3> fArray;
	fArray.reserve(Mesh->vertices.size());

	for (const auto& vertex : Mesh->vertices)
		fArray.push_back(vertex.Position);

	aabb.SetFrom(&fArray[0], fArray.size());
}

void ComponentMesh::UpdateBoundingBoxes()
{
	obb = aabb;
	obb.Transform(parent->transform->GetGlobalTransform());

	gAABB.SetNegativeInfinity();
	gAABB.Enclose(obb);
}

void ComponentMesh::RenderBoundingBoxes()
{
	float3 verticesOBB[8];
	obb.GetCornerPoints(verticesOBB);
	DrawBox(verticesOBB, float3(255, 0, 0));

	float3 verticesAABB[8];
	gAABB.GetCornerPoints(verticesAABB);
	DrawBox(verticesAABB, float3(0, 0, 255));
}

void ComponentMesh::DrawBox(float3* vertices, float3 color)
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

	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); i++) 
	{
		glVertex3fv(vertices[indices[i]].ptr());
	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();
}

void ComponentMesh::EnableBB()
{
	this->showBB = true;
}

void ComponentMesh::DisableBB()
{
	this->showBB = false;
}

void ComponentMesh::ToggleBB()
{
	this->showBB = !this->showBB;
}