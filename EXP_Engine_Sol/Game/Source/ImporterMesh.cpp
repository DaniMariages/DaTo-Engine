#include "Application.h"
#include "ImporterMesh.h"
#include "ModuleFileSystem.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleScene.h"

#include "../External/Assimp/include/mesh.h"
#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"

#pragma comment (lib, "Game/External/Assimp/libx86/assimp.lib")

void Importer::MeshImporter::LoadMeshes(const aiMesh* Mesh, GameObject* gameObject, const char* file_path)
{
	ResourceMesh* myMesh = new ResourceMesh();
	ComponentMesh* compMesh = new ComponentMesh(gameObject);

	for (unsigned int j = 0; j < Mesh->mNumVertices; j++)
	{
		Vertex vertex;
		float3 vector;
		vector.x = Mesh->mVertices[j].x;
		vector.y = Mesh->mVertices[j].y;
		vector.z = Mesh->mVertices[j].z;
		vertex.Position = vector;

		if (Mesh->HasNormals())
		{
			vertex.Normal.x = Mesh->mNormals[j].x;
			vertex.Normal.y = Mesh->mNormals[j].y;
			vertex.Normal.z = Mesh->mNormals[j].z;

		}
		if (Mesh->HasTextureCoords(0))
		{
			vertex.TexCoords.x = Mesh->mTextureCoords[0][j].x;
			vertex.TexCoords.y = Mesh->mTextureCoords[0][j].y;
		}
		else
		{
			vertex.TexCoords.x = 0.0f;
			vertex.TexCoords.y = 0.0f;
		}

		myMesh->vertices.push_back(vertex);
	}

	if (Mesh->HasFaces())
	{
		myMesh->indices.resize(Mesh->mNumFaces * 3);	//assume each face is a triangle

		for (uint y = 0; y < Mesh->mNumFaces; y++)
		{
			if (Mesh->mFaces[y].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&myMesh->indices[y * 3], Mesh->mFaces[y].mIndices, 3 * sizeof(unsigned int));
			}
		}
	}

	compMesh->SetPath(std::string(file_path));
	compMesh->SetMesh(myMesh);
	compMesh->InitBoundingBoxes(myMesh);

	myMesh->SetUpBuffers(myMesh);
	gameObject->AddComponent(compMesh);
	
	//SetUpBuffers(myMesh);
	//meshes.push_back(*myMesh);*/

    ///*char* buffer = nullptr;
    //uint size = Importer::MeshImporter::Save(myMesh, &buffer);
}

void Importer::MeshImporter::Load(ResourceMesh* Mesh, const char* buffer)
{
    char* cursor = const_cast<char*>(buffer);

    uint ranges[2];
    uint bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    Mesh->indices.resize(ranges[0]);
    Mesh->vertices.resize(ranges[1]);

    bytes = sizeof(uint) * Mesh->indices.size();
    memcpy(Mesh->indices.data(), cursor, bytes);
    cursor += bytes;

    bytes = sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 2 * Mesh->vertices.size();
    memcpy(Mesh->vertices.data(), cursor, bytes);
    cursor += bytes;
}

uint Importer::MeshImporter::Save(const mesh* Mesh, char** buffer)
{
    uint headerSize = sizeof(uint) * 2; 
    uint vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2;

    uint size = headerSize + sizeof(uint) * Mesh->indices.size() + vertexSize * Mesh->vertices.size();

    *buffer = new char[size];
    char* cursor = *buffer;

    uint ranges[2] = { Mesh->indices.size(), Mesh->vertices.size() };
    uint bytes = headerSize;
    memcpy(cursor, ranges, bytes);
    cursor += bytes;

    bytes = sizeof(uint) * Mesh->indices.size();
    memcpy(cursor, Mesh->indices.data(), bytes);
    cursor += bytes;

    bytes = sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 2 * Mesh->vertices.size();
    memcpy(cursor, Mesh->vertices.data(), bytes);
    cursor += bytes;

    return size;
}

void Importer::MeshImporter::Load(mesh* Mesh, const char* buffer)
{
    char* cursor = const_cast<char*>(buffer);

    uint ranges[2];
    uint bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    Mesh->indices.resize(ranges[0]);
    Mesh->vertices.resize(ranges[1]);

    bytes = sizeof(uint) * Mesh->indices.size();
    memcpy(Mesh->indices.data(), cursor, bytes);
    cursor += bytes;

    bytes = sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 2 * Mesh->vertices.size();
    memcpy(Mesh->vertices.data(), cursor, bytes);
    cursor += bytes;

    ExternalApp->importer->SetUpBuffers(Mesh);
}
