#pragma once
#include "Globals.h"
#include "ModuleImport.h"

struct aiMaterial;
class ResourceMaterial;
class ResourceTexture;
class Texture;

namespace Importer
{
	/*namespace MaterialsImporter
	{
		void ImportMaterial(aiMaterial* material, ResourceMaterial* resourceMaterial);
		uint Save(ResourceMaterial* rMaterial, char** buffer);
		void Load(ResourceMaterial* rMaterial, const char* buffer);
	}*/

	namespace TextureImporter
	{
		void InitDevil();
		void ImportTexture(ResourceTexture* rMaterial, const char* buffer, uint size);
		uint Save(const ResourceTexture* rMaterial, char** buffer); // Save in ResourceTexture
		void Load(ResourceTexture* rMaterial, char* buffer, uint size); // Load from ResourceTexture

		void ImportTexture(Texture* texture, const char* buffer, uint size);
		uint Save(const Texture* texture, char** buffer); // Save in Texture
		void Load(Texture* texture, char* buffer, uint size); // Load from Texture
	}
}
