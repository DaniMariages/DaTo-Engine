#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"
#include "ModuleImport.h"
#include "../External/ImGui/imgui.h"
#include <string>

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	texture = new Texture();
	texture->textID = -1;
	texture->height = -1;
	texture->width = -1;
	type = typeComponent::Material;
}

ComponentTexture::~ComponentTexture() {}

void ComponentTexture::Enable() {}
void ComponentTexture::Disable() {}
void ComponentTexture::Update() {}

void ComponentTexture::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Texture"))
	{
		ImGui::Text("Id: %d", texture->textID);
	}
}

void ComponentTexture::SetTexture(Texture* texture)
{
	this->texture->textID = texture->textID;
	this->texture->height = texture->height;
	this->texture->width = texture->width;
	this->texture->path = texture->path;
}

void ComponentTexture::SetTexture(uint id, const char* path)
{
	this->texture->textID = texture->textID;
	this->texture->height = texture->height;
	this->texture->width = texture->width;
	this->texture->path = texture->path;
}

Texture* ComponentTexture::GetTexture()
{
	if (this != nullptr) {
		Texture ret;

		ret.textID = this->texture->textID;
		ret.height = this->texture->height;
		ret.width = this->texture->width;
		ret.path = this->texture->path;

		return &ret;
	}
	return nullptr;
}
