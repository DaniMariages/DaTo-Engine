#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "ResourceMaterial.h"

#include "../External/ImGui/imgui.h"

#include <string>

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	rMaterial = new ResourceMaterial();
	rMaterial->SetId(-1);
	rMaterial->SetHeight(-1);
	rMaterial->SetWidth(-1);
	type = typeComponent::Material;
}

ComponentTexture::~ComponentTexture() {}

void ComponentTexture::Enable() {}
void ComponentTexture::Disable() {}
void ComponentTexture::Update() {}

void ComponentTexture::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Texture"))
	{
		ImGui::Text("Id: %d.", rMaterial->GetId());
		ImGui::Text("Height: %d.", rMaterial->GetHeight());
		ImGui::Text("Width: %d.", rMaterial->GetWidth());
		ImGui::Text("Path: %s.", rMaterial->GetPath().c_str());
		ImGui::Image((ImTextureID)rMaterial->GetId(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
	}
}

//void ComponentTexture::SetTexture(Texture* texture)
//{
//	this->texture->textID = texture->textID;
//	this->texture->height = texture->height;
//	this->texture->width = texture->width;
//	this->texture->path = texture->path;
//}
//
//Texture* ComponentTexture::GetTexture()
//{
//	if (this != nullptr) {
//		Texture ret;
//
//		ret.textID = this->texture->textID;
//		ret.height = this->texture->height;
//		ret.width = this->texture->width;
//		ret.path = this->texture->path;
//
//		return &ret;
//	}
//	return nullptr;
//}
