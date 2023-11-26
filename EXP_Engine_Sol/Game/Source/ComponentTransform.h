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

	void SetPosition(float3 position);
	void SetRotation(Quat rotation);
	void SetScale(float3 scale);

	float3 GetUp();
	float3 GetForward();

	float3 GetPosition() const { return this->position; };
	Quat GetRotation() const { return this->rotation; };
	float3 GetScale() const { return this->scale; };

	float4x4 GetTransformMatrix() const { return this->transform; };
	float4x4 SetTransformMatrix(float4x4 transformMatrix);

	ComponentTransform(GameObject* parent);
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);

	bool updatedTransform = false;

};
