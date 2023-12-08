#pragma once
#include "Component.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {

private:
	float3 position;
	Quat rotation;
	float3 eulerRotation;
	float3 scale;

	float3 gPosition;
	Quat gRotation;
	float3 gEulerRotation;
	float3 gScale;

	float4x4 transform = float4x4::identity;
	float4x4 gTransform = float4x4::identity;

	void UpdateMatrix();

public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;

	void SetPosition(float3 position);
	void SetRotation(Quat rotation);
	void SetEulerRotation(float3 rotation);
	void SetScale(float3 scale);

	void SetGlobalPosition(float3 position);
	void SetGlobalRotation(Quat rotation);
	void SetGlobalEulerRotation(float3 rotation);
	void SetGlobalScale(float3 scale);

	float3 GetUp();
	float3 GetForward();

	float3 GetPosition() const { return this->gPosition; };
	Quat GetRotation() const { return this->gRotation; };
	float3 GetScale() const { return this->gScale; };

	float4x4 GetGlobalTransform() const { return this->gTransform; }
	float4x4 GetLocalTransform() const { return this->transform; }
	float4x4 SetLocalTransform(float4x4 transformMatrix);

	void UpdateTransform();

	ComponentTransform(GameObject* parent);
	ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation);
};
