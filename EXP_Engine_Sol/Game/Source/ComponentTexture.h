#pragma once
#include "Component.h"
#include "Globals.h"
#include "ModuleImport.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"

class ResourceMaterial;

class ComponentTexture : public Component
{
private:
	ResourceMaterial* rMaterial = nullptr;

public:
	ComponentTexture(GameObject* parent);
	~ComponentTexture();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	ResourceMaterial* GetrMaterial() const { return rMaterial; };
	Texture* GetTexture();
	void SetTexture(Texture* texture);
};
