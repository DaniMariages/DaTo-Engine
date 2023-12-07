#include "Application.h"
#include "ImporterMesh.h"
#include "ModuleFileSystem.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleScene.h"
//#include "ResourceMesh.h"

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

void Importer::MeshImporter::Load(ResourceMesh* mesh, const char* buffer)
{
    //const char* cursor = buffer;
    //uint bytes;

    //uint ranges[4];
    //bytes = sizeof(ranges);
    //memcpy(ranges, cursor, bytes);
    //cursor += bytes;

    //mesh->size[ResourceMesh::index] = ranges[0];
    //mesh->size[ResourceMesh::vertex] = ranges[1];
    //mesh->size[ResourceMesh::normal] = ranges[2];
    //mesh->size[ResourceMesh::texture] = ranges[3];

    //// Load indices
    //bytes = sizeof(uint) * mesh->size[ResourceMesh::index];
    //mesh->indices = new uint[mesh->size[ResourceMesh::index]];
    //memcpy(mesh->indices, cursor, bytes);
    //cursor += bytes;

    //// Load vertices
    //bytes = sizeof(float) * mesh->size[ResourceMesh::vertex] * 3;
    //mesh->vertices = new float[mesh->size[ResourceMesh::vertex] * 3];
    //memcpy(mesh->vertices, cursor, bytes);
    //cursor += bytes;

    //// Load normals
    //if (mesh->size[ResourceMesh::normal] > 0)
    //{
    //    bytes = sizeof(float) * mesh->size[ResourceMesh::normal] * 3;
    //    mesh->normals = new float[mesh->size[ResourceMesh::normal] * 3];
    //    memcpy(mesh->normals, cursor, bytes);
    //    cursor += bytes;
    //}

    //// Load Texture Coordinates
    //if (mesh->size[ResourceMesh::texture] > 0)
    //{
    //    bytes = sizeof(float) * mesh->size[ResourceMesh::texture] * 2;
    //    mesh->texCoords = new float[mesh->size[ResourceMesh::texture] * 2];
    //    memcpy(mesh->texCoords, cursor, bytes);
    //    cursor += bytes;
    //}

    //mesh->SetUpBuffers(mesh);
    //mesh->CreateAABB();

}

uint Importer::MeshImporter::Save(const mesh* Mesh, char** buffer)
{
    // Define the size of the header (ranges) and data (vertices)
    uint headerSize = sizeof(uint) * 2; // num_indices and num_vertices
    uint vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2; // size of Vertex structure

    // Calculate the total size of the file buffer
    uint size = headerSize + sizeof(uint) * Mesh->indices.size() + vertexSize * Mesh->vertices.size();

    // Allocate memory for the file buffer
    *buffer = new char[size];

    // Create a cursor to keep track of the current position in the buffer
    char* cursor = *buffer;

    // Store the header (ranges)
    uint ranges[2] = { Mesh->indices.size(), Mesh->vertices.size() };
    uint bytes = headerSize;
    memcpy(cursor, ranges, bytes);
    cursor += bytes;

    // Store the indices
    bytes = sizeof(uint) * Mesh->indices.size();
    memcpy(cursor, Mesh->indices.data(), bytes);
    cursor += bytes;

    // Store the vertices
    bytes = sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 2 * Mesh->vertices.size();
    memcpy(cursor, Mesh->vertices.data(), bytes);
    cursor += bytes;

    // Return the size of the file buffer
    return size;
}

void Importer::MeshImporter::Load(mesh* Mesh, const char* buffer)
{
    char* cursor = const_cast<char*>(buffer);

    // Load the header (ranges)
    uint ranges[2];
    uint bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    // Resize indices and vertices
    Mesh->indices.resize(ranges[0]);
    Mesh->vertices.resize(ranges[1]);

    // Load indices
    bytes = sizeof(uint) * Mesh->indices.size();
    memcpy(Mesh->indices.data(), cursor, bytes);
    cursor += bytes;

    // Load vertices
    bytes = sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 3 * Mesh->vertices.size() + sizeof(float) * 2 * Mesh->vertices.size();
    memcpy(Mesh->vertices.data(), cursor, bytes);
    cursor += bytes;

    ExternalApp->importer->SetUpBuffers(Mesh);
}
