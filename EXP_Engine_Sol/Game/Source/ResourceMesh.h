#pragma once
#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "Globals.h"
#include "Resource.h"

#include "ModuleImport.h"

#include "../External/MathGeoLib/include/MathGeolib.h"

class ResourceMesh : public Resource
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	std::vector<ResourceMesh> meshes;
	
public:

	ResourceMesh();
	ResourceMesh(const char* assetsFile, const char* libraryFile, const char* name, uint UID);
	~ResourceMesh();

	void DrawNormals();
	void DrawTexCoords();
	void SetUpBuffers(ResourceMesh* mesh);

};
#endif //__ResourceMesh_H__