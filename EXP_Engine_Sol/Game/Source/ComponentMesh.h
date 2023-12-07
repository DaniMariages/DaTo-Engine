#pragma once
#include "Application.h"
#include "Component.h"
#include "ModuleImport.h"
#include "ResourceMesh.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/MathGeoLib/include/Geometry/OBB.h"


struct mesh;

class ComponentMesh : public Component 
{
public: 
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	mesh* GetMesh() const { return Mesh; };
	std::string GetPath() const { return path; };

	void SetMesh(mesh* Mesh);
	void SetMesh(ResourceMesh* Mesh);

	void SetPath(std::string path);

	void InitBoundingBoxes(mesh* Mesh);
	void InitBoundingBoxes(ResourceMesh* Mesh);

	void UpdateBoundingBoxes();
	void RenderBoundingBoxes();
	void DrawBox(float3* vertices, float3 color);

	AABB gAABB;

	AABB aabb;
	OBB obb;

	void EnableBB();
	void DisableBB();
	void ToggleBB();
	bool showBB = true;

private:
	
	mesh* Mesh;
	ResourceMesh* rMesh = nullptr;
	std::string path;

};
