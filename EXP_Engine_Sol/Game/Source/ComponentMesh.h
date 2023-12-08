#pragma once
#include "Application.h"
#include "Component.h"
#include "ModuleImport.h"
#include "ResourceMesh.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"
#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/MathGeoLib/include/Geometry/OBB.h"

class ResourceMesh;

class ComponentMesh : public Component 
{
public: 
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	ResourceMesh* GetrMesh() const { return rMesh; };
	std::string GetPath() const { return path; };

	void SetMesh(ResourceMesh* Mesh);
	void SetPath(std::string path);

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
	
	ResourceMesh* rMesh = nullptr;
	std::string path;

};
