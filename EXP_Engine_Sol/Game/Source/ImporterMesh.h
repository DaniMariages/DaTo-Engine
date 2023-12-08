#pragma once
#include "Globals.h"

#include "ResourceMesh.h"

namespace Importer
{
	namespace MeshImporter
	{
		void LoadMeshes(ResourceMesh* rMesh, const aiMesh* Mesh, GameObject* gameObject, const char* file_path);	
		uint Save(const ResourceMesh* mesh, char** buffer);
		void Load(ResourceMesh* mesh, const char* buffer); 
	}
}
