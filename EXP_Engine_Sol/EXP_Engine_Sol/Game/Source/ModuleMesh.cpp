#include "ModuleMesh.h"

#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"

#pragma comment (lib, "Game/External/Assimp/libx86/assimp.lib")

#include <vector>


ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled) {}

bool ModuleMesh::Start() { return true; }

void ModuleMesh::LoadMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh Mesh;

			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
			{
				Vertex vertex;
				float3 vector;
				vector.x = scene->mMeshes[i]->mVertices[j].x;
				vector.y = scene->mMeshes[i]->mVertices[j].y;
				vector.z = scene->mMeshes[i]->mVertices[j].z;
				vertex.Position = vector;

				if (scene->mMeshes[i]->HasNormals())
				{
					vertex.Normal.x = scene->mMeshes[i]->mNormals[j].x;
					vertex.Normal.y = scene->mMeshes[i]->mNormals[j].y;
					vertex.Normal.z = scene->mMeshes[i]->mNormals[j].z;
				}
				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					vertex.TexCoords.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
					vertex.TexCoords.y = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
				else
				{
					vertex.TexCoords.x = 0.0f;
					vertex.TexCoords.y = 0.0f;
				}

				Mesh.vertices.push_back(vertex);
			}

			if (scene->mMeshes[i]->HasFaces())
			{
				Mesh.indices.resize(scene->mMeshes[i]->mNumFaces * 3);	//assume each face is a triangle

				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&Mesh.indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));
					}
				}
			}

			meshes.push_back(Mesh);
		}

		LOG("Scene loaded correctly");
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene % s", file_path);
}

ModuleMesh::~ModuleMesh() {}

update_status ModuleMesh::Update(float dt) { return UPDATE_CONTINUE; }

bool ModuleMesh::CleanUp() { return true; }