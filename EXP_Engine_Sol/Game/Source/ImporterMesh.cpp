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

void Importer::MeshImporter::LoadMeshes(ResourceMesh* mesh, const aiMesh* aiMesh)
{
    //mesh->size[ResourceMesh::vertex] = aiMesh->mNumVertices;
    //mesh->vertices = new float[mesh->size[ResourceMesh::vertex] * 3];
    //memcpy(mesh->vertices, aiMesh->mVertices, sizeof(float) * mesh->size[ResourceMesh::vertex] * 3);
    //LOG("New mesh with %d vertices", mesh->size[ResourceMesh::vertex]);

    //mesh->CreateAABB();

    //if (aiMesh->HasFaces())
    //{
    //    mesh->size[ResourceMesh::index] = aiMesh->mNumFaces * 3;
    //    mesh->indices = new uint[mesh->size[ResourceMesh::index]];
    //    for (uint f = 0; f < aiMesh->mNumFaces; ++f)
    //    {
    //        if (aiMesh->mFaces[f].mNumIndices != 3)
    //        {
    //            LOG("Mesh face with less or more than 3 indices!");
    //        }
    //        else
    //        {
    //            memcpy(&mesh->indices[f * 3], aiMesh->mFaces[f].mIndices, 3 * sizeof(uint));
    //        }
    //    }

    //    LOG("With %d indices", mesh->size[ResourceMesh::index]);
    //}

    //if (aiMesh->HasNormals())
    //{
    //    mesh->size[ResourceMesh::normal] = aiMesh->mNumVertices;
    //    mesh->normals = new float[mesh->size[ResourceMesh::normal] * 3];
    //    memcpy(mesh->normals, aiMesh->mNormals, sizeof(float) * mesh->size[ResourceMesh::normal] * 3);
    //}

    //if (aiMesh->HasTextureCoords(0))
    //{
    //    mesh->size[ResourceMesh::texture] = aiMesh->mNumVertices;
    //    mesh->texCoords = new float[aiMesh->mNumVertices * 2];

    //    for (int j = 0; j < mesh->size[ResourceMesh::texture]; j++)
    //    {
    //        mesh->texCoords[j * 2] = aiMesh->mTextureCoords[0][j].x;
    //        mesh->texCoords[j * 2 + 1] = aiMesh->mTextureCoords[0][j].y;
    //    }
    //}



    //mesh->SetUpBuffers(mesh); // This have to go out
    ////meshes.push_back(tempMesh);

    ///*char* buffer = nullptr;
    //uint64 size = Importer::MeshImporter::Save(tempMesh, &buffer);*/
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
