#pragma once
#include "Application.h"
#include "Component.h"
#include "ModuleImport.h"

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
	void SetPath(std::string path);

	void InitBoundingBoxes(mesh* Mesh);
	void UpdateBoundingBoxes();
	void RenderBoundingBoxes();
	void DrawBox(float3* vertices, float3 color);

	AABB gAABB;

	AABB aabb;
	OBB obb;

private:
	
	mesh* Mesh;

	std::string path;
};
