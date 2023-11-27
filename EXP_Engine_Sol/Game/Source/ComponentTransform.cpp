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

void ComponentTransform::Enable() { if (!this->active) this->active = true; }

void ComponentTransform::Disable() { if (this->active) this->active = false; }

void ComponentTransform::Update() 
{
	//MYTODO: I think is better get this here instead GameObject Update
	//if (ExternalApp->scene->gameObjectSelected != nullptr && ExternalApp->scene->gameObjectSelected != ExternalApp->scene->rootGameObject 
	//	&& !ExternalApp->scene->gameObjectSelected->children.empty())
	//{
	//	//to dont get a kilometer function
	//	GameObject* gameObjectSelected = ExternalApp->scene->gameObjectSelected;
	//	
	//	for (unsigned int i = 0; i < gameObjectSelected->children.size(); i++)
	//	{
	//		if (gameObjectSelected->children[i]->GetComponent(typeComponent::Mesh)->type == typeComponent::Mesh)
	//		{
	//			ComponentTransform* compMesh = (ComponentTransform*)gameObjectSelected->children[i]->GetComponent(typeComponent::Transform);
	//			compMesh->SetTransformMatrix(gameObjectSelected->transform->GetTransformMatrix());
	//		}
	//	}
	//}
}

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

float4x4 ComponentTransform::SetTransformMatrix(float4x4 transformMatrix)
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

		//This button reset all transformations
		if (ImGui::Button("Reset Transformations")) 
		{
			SetPosition(float3(0.0f, 0.0f, 0.0f));
			SetRotation(Quat::FromEulerXYZ(0.0f, 0.0f, 0.0f));
			SetScale(float3(1.0f, 1.0f, 1.0f));
		}

		UpdateMatrix();
	}
}
