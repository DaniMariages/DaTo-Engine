#pragma once
#include "Component.h"
#include "ModuleImport.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

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

private:
	
	mesh* Mesh;

	std::string path;
};
