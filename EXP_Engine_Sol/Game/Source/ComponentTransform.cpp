#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"

#include "Globals.h"
#include "../External/ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	position = float3(0.0f, 0.0f, 0.0f);
	rotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f);
	eulerRotation = float3(0.0f, 0.0f, 0.0f);
	scale = float3(1.0f, 1.0f, 1.0f);

	gPosition = float3(0.0f, 0.0f, 0.0f);
	gRotation = Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f);
	gEulerRotation = float3(0.0f, 0.0f, 0.0f);
	gScale = float3(1.0f, 1.0f, 1.0f);

	type = typeComponent::Transform;
	UpdateMatrix();
};

ComponentTransform::ComponentTransform(GameObject* parent, float3 _position, float3 _scale, Quat _rotation) : Component(parent)
{
	gPosition = _position;
	gRotation = _rotation;
	gScale = _scale;

	transform = float4x4::FromTRS(gPosition, gRotation, gScale);
	type = typeComponent::Transform;
};

void ComponentTransform::Enable() { if (!this->active) this->active = true; }

void ComponentTransform::Disable() { if (this->active) this->active = false; }

void ComponentTransform::Update() {}

void ComponentTransform::SetGlobalPosition(float3 position)
{
	float3 parentPosition = float3::zero;
	float3 newPos;

	if (parent->Parent != nullptr) parentPosition = parent->Parent->transform->GetPosition();
	newPos = position - parentPosition;
	SetPosition(newPos);
}

void ComponentTransform::SetGlobalRotation(Quat rotation)
{
	Quat parentRotation = Quat::identity;
	Quat newRot;

	if (parent->Parent != nullptr) parentRotation = parent->Parent->transform->GetRotation();
	newRot = rotation.Mul(parentRotation);
	SetRotation(newRot);
}

void ComponentTransform::SetGlobalEulerRotation(float3 _rotation)
{
	Quat qRot = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
	SetGlobalRotation(qRot);
}

void ComponentTransform::SetGlobalScale(float3 scale)
{
	float3 parentScale = float3::one;
	float3 newSca;

	if (parent->Parent != nullptr) parentScale = parent->Parent->transform->GetScale();
	newSca = scale - parentScale;
	SetScale(newSca);
}

void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateMatrix();
	UpdateTransform();
};

void ComponentTransform::SetRotation(Quat _rotation)
{
	this->rotation = _rotation;
	this->eulerRotation = rotation.ToEulerXYZ();
	UpdateMatrix();
	UpdateTransform();
};

void ComponentTransform::SetEulerRotation(float3 _rotation)
{
	eulerRotation = _rotation;
	rotation = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
	UpdateMatrix();
	UpdateTransform();
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateMatrix();
	UpdateTransform();
};

float4x4 ComponentTransform::SetLocalTransform(float4x4 transformMatrix)
{
	return transform = transformMatrix;
}

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

void ComponentTransform::UpdateTransform()
{
	// Create a list with transforms to update
	std::vector<ComponentTransform*> updateTransform;
	updateTransform.push_back(this);

	// While list is not empty
	while (!updateTransform.empty())
	{
		// Create a auxiliar ComponentTransform
		ComponentTransform* tempTrans = *updateTransform.begin();
		updateTransform.erase(updateTransform.begin());

		// For all the childrens of parent get their transform and save it in the list
		for (unsigned int i = 0; i < tempTrans->parent->children.size(); i++)
		{
			updateTransform.push_back(tempTrans->parent->children[i]->transform);
		}

		// Create a new auxiliar matrix for the parent
		float4x4 parentWorldMatrix = float4x4::identity;

		// If parent gameObject is not null
		if (tempTrans->parent->Parent != nullptr)
		{
			// If has component Transform
			if (tempTrans->parent->Parent->HasComponent(typeComponent::Transform))
				// Set the auxiliar matrix as parent matrix transform
				parentWorldMatrix = tempTrans->parent->Parent->transform->GetGlobalTransform();
		}

		tempTrans->gTransform = parentWorldMatrix * tempTrans->transform;
		tempTrans->gTransform.Decompose(gPosition, gRotation, gScale);
		this->gEulerRotation = gRotation.ToEulerXYZ();
	}
}

void ComponentTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		float3 newGlobalPosition = position;
		float3 newGlobalScale = scale;
		float3 newGlobalRotation = eulerRotation * RADTODEG;

		bool positionChanged = ImGui::DragFloat3("Position:", (float*)&newGlobalPosition);
		bool scaleChanged = ImGui::DragFloat3("Scale:", (float*)&newGlobalScale);
		bool rotationChanged = ImGui::DragFloat3("Rotation:", (float*)&newGlobalRotation);

		if (positionChanged) SetPosition(newGlobalPosition);

		if (scaleChanged) SetScale(newGlobalScale);

		if (rotationChanged)
		{
			newGlobalRotation *= DEGTORAD;
			SetEulerRotation(newGlobalRotation);
		}

		//This button reset all transformations
		if (ImGui::Button("Reset Transformations"))
		{
			SetPosition(float3(0.0f, 0.0f, 0.0f));
			SetRotation(Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f));
			SetScale(float3(1.0f, 1.0f, 1.0f));
		}
	}
}
