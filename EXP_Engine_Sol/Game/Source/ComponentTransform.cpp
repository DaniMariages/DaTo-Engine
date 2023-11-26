#include "ComponentTransform.h"
#include "GameObject.h"

#include "Globals.h"
#include "../External/ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	position = float3(0.0f, 0.0f, 0.0f);
	rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f);
	scale = float3(1.0f, 1.0f, 1.0f);

	type = typeComponent::Transform;
	UpdateMatrix();
};

ComponentTransform::ComponentTransform(GameObject* parent, float3 _position, float3 _scale, Quat _rotation) : Component(parent)
{
	position = _position;
	rotation = _rotation;
	scale = _scale;

	transform = float4x4::FromTRS(position, rotation, scale);
	type = typeComponent::Transform;
};

void ComponentTransform::Enable() {	if (!this->active) this->active = true;	}

void ComponentTransform::Disable() { if (this->active) this->active = false; }

void ComponentTransform::Update() {}

void ComponentTransform::SetPosition(float3 position) 
{
	this->position = position;  
	UpdateMatrix(); 
};

void ComponentTransform::SetRotation(Quat rotation)
{ 
	this->rotation = rotation; 
	UpdateMatrix(); 
};

void ComponentTransform::SetScale(float3 scale)
{ 
	this->scale = scale; 
	UpdateMatrix(); 
};

float3 ComponentTransform::GetUp()
{
	return transform.RotatePart().Col(1).Normalized();
}

float3 ComponentTransform::GetForward()
{
	return transform.RotatePart().Col(2).Normalized();
}

void ComponentTransform::UpdateMatrix() 
{
	transform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::DrawInspector() 
{
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		float3 eulerRotation = rotation.ToEulerXYZ();
		float3 eulerDegree = eulerRotation * RADTODEG;
		ImGui::DragFloat3("Position:", (float*)&position);
		ImGui::DragFloat3("Scale:", (float*)&scale);
		bool rotationChanged = ImGui::DragFloat3("Rotation:", (float*)&eulerDegree);

		if (rotationChanged)
		{
			eulerRotation = eulerDegree * DEGTORAD;
			rotation = Quat::FromEulerXYZ(eulerRotation.x, eulerRotation.y, eulerRotation.z);
		}

		UpdateMatrix();
	}
}
