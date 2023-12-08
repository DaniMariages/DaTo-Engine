#pragma once
#include "Component.h"
#include "Globals.h"
#include "ModuleImport.h"
#include "ResourceTexture.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"

struct Texture;

class ComponentTexture : public Component
{
private:
	Texture* texture;
	ResourceTexture* rTexture;

public:
	ComponentTexture(GameObject* parent);
	~ComponentTexture();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	ResourceTexture* GetrTexture() const { return rTexture; };
	Texture* GetTexture();
	void SetTexture(Texture* texture);

};
