#pragma once
#include "Globals.h"

#include "ModuleImport.h"
#include "ResourceMesh.h"

namespace Importer
{
	namespace MeshImporter
	{
		void LoadMeshes(const aiMesh* aiMesh, GameObject* gameObject, const char* file_path);
		void LoadMeshes(ResourceMesh* rMesh, const aiMesh* Mesh, GameObject* gameObject, const char* file_path);
		
		uint Save(const ResourceMesh* mesh, char** buffer); //Save with ResourceMesh
		uint Save(const mesh* mesh, char** buffer); //Save with mesh
		
		void Load(ResourceMesh* mesh, const char* buffer); //Load mesh from ResourceMesh
		void Load(mesh* mesh, const char* buffer); //Load mesh from mesh
	}
}
