#pragma once
#include "Component.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {

private:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 transform;
	void UpdateMatrix();

public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	void inline SetPosition(float3 position);
	void inline SetRotation(Quat rotation);
	void inline SetScale(float3 scale);

	float3 inline GetPosition() const { return this->position; };
	Quat inline GetRotation() const { return this->rotation; };
	float3 inline GetScale() const { return this->scale; };
	float4x4 inline GetTransformMatrix() const { return this->transform; };

	ComponentTransform(GameObject* parent);
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);

};
