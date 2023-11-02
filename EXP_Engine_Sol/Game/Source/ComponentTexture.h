#pragma once
#include "Component.h"
#include "Globals.h"
#include "ModuleImport.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

struct Texture;

class ComponentTexture : public Component
{
private:
	Texture* texture;

public:
	ComponentTexture(GameObject* parent);
	~ComponentTexture();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	Texture* GetTexture();

	void SetTexture(Texture* texture);
	void SetTexture(uint id, const char* path);

};
