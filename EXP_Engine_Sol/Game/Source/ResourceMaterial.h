#pragma once
#include "Globals.h"
#include "Color.h"
#include "Resource.h"
#include "ResourceTexture.h"

class ResourceMaterial : public Resource
{
public:

	ResourceMaterial(const char* assetsFile, const char* libraryFile, const char* name, uint UID);
	ResourceMaterial(uint _id, const char* _path);
	ResourceMaterial(ResourceTexture* texture);
	ResourceMaterial();
	~ResourceMaterial();

	ResourceTexture* GetTexture() { return texture; }
	Color GetColor() const { return color; }
	uint GetId() { return texture->id; }
	std::string GetPath() const { return texture->path; }
	uint GetHeight() { return texture->height; }
	uint GetWidth() { return texture->width; }

	void SetColor(Color color) { this->color = color; }

	void SetTexture(ResourceTexture* rTexture) { this->texture = rTexture; }
	void SetTexture(uint id, const char* path) { this->texture->id = id; this->texture->path = path; }

	void SetId(uint id) { this->texture->id = id; }
	void SetPath(std::string path) { this->texture->path = path; }
	void SetHeight(uint height) { this->texture->height = height; }
	void SetWidth(uint width) { this->texture->width = width; }

private:

	ResourceTexture* texture = nullptr;
	Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);

};