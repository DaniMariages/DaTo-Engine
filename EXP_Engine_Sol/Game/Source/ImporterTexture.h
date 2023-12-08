#pragma once
#include "Globals.h"

struct aiMaterial;
class ResourceMaterial;
class ResourceTexture;
class Texture;

namespace Importer
{
	namespace MaterialsImporter
	{
		void ImportMaterial(aiMaterial* material, ResourceMaterial* resourceMaterial);
		uint Save(ResourceMaterial* rMaterial, char** buffer);
		void Load(ResourceMaterial* rMaterial, const char* buffer);
	}

	namespace TextureImporter
	{
		void ImportTexture(ResourceTexture* rMaterial, const char* buffer, uint size);
		uint Save(const ResourceTexture* rMaterial, char** buffer);
		void Load(ResourceTexture* rMaterial, char* buffer, uint size);
	}
}
