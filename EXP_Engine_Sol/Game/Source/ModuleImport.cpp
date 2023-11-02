#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"

#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"

#pragma comment(lib, "Game/External/DevIL/libx86/DevIL.lib")
#pragma comment(lib, "Game/External/DevIL/libx86/ILU.lib")
#pragma comment(lib, "Game/External/DevIL/libx86/ILUT.lib")

#pragma comment (lib, "Game/External/Assimp/libx86/assimp.lib")

#include <vector>

ModuleImport::ModuleImport(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleImport::~ModuleImport() {}

bool ModuleImport::Start() { return true; }

bool ModuleImport::CleanUp() { return true; }

update_status ModuleImport::Update(float dt) { return UPDATE_CONTINUE; }

void ModuleImport::ReadFile(const char* file_path) 
{ 
	LOG("Reading file, path: %s.", file_path);

	name = GetName(file_path);
	path = file_path;
	typeFile extension = ReadExtension(name);

	switch (extension) {
	case typeFile::MODEL:
		LOG("START LOADING MODEL");
		BakerHouse = new GameObject(name);
		LoadMesh(file_path);
		LOG("MODEL LOADED");
		App->renderer3D->gameObjects.push_back(BakerHouse);
		break;

	case typeFile::TEXTURE:
		LOG("START LOADING TEXTURE");
		LoadTexture(file_path);
		ComponentTexture* tempCompTex = new ComponentTexture(BakerHouse);
		tempCompTex->SetTexture(LoadTexture(file_path));
		LOG("TEXTURE LOADED");
		BakerHouse->AddComponent(tempCompTex);
		break;
	}
}

std::string ModuleImport::GetName(const char* file_path)
{
	std::string getName(file_path);
	size_t lastSeparator = getName.find_last_of("/\\");

	if (lastSeparator != std::string::npos)
	{
		name = getName.substr(lastSeparator + 1);
	}
	else
	{
		name = getName;
	}

	LOG("The file name is: %s", name.c_str());
	return name;
}

typeFile ModuleImport::ReadExtension(std::string name) 
{
	size_t lastSeparator = name.find_last_of(".");

	std::string extension;
	if (lastSeparator != std::string::npos)
	{
		extension = name.substr(lastSeparator + 1);
	}
	else extension = name;

	typeFile typeExtension = typeFile::UNKNOWN;

	if (extension == "fbx" || extension == "FBX")
	{
		typeExtension = typeFile::MODEL;
		LOG("File dropped is: Model");
	}
	else if (extension == "png")
	{
		typeExtension = typeFile::TEXTURE;
		LOG("File dropped is: Texture");
	}
	else if (extension == "dds") {
		typeExtension = typeFile::TEXTURE;
		LOG("File dropped is: Texture");
	}

	LOG("La extension del modelo es: %s", extension.c_str());
	return typeExtension;
}

void ModuleImport::LoadMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh* Mesh = new mesh();
 			ComponentMesh* compMesh = new ComponentMesh(BakerHouse);

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
				
				Mesh->vertices.push_back(vertex);
			}

			if (scene->mMeshes[i]->HasFaces())
			{
				Mesh->indices.resize(scene->mMeshes[i]->mNumFaces * 3);	//assume each face is a triangle

				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&Mesh->indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));
					}
				}
			}
			compMesh->SetPath(std::string(file_path));
			compMesh->SetMesh(Mesh);

			BakerHouse->AddComponent(compMesh);
			App->renderer3D->SetUpBuffers(Mesh);
			meshes.push_back(*Mesh);
		}

		LOG("Scene loaded correctly");
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene % s", file_path);
}

Texture* ModuleImport::LoadTexture(const char* file_path)
{
	ILuint image;
	GLboolean imageLoaded;

	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoadImage(file_path))
	{
		ILinfo ImageInfo;
		GLuint texture_id;

		iluGetImageInfo(&ImageInfo);
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (ImageInfo.Origin == IL_ORIGIN_LOWER_LEFT)
		{
			iluFlipImage();
		}

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ImageInfo.Width, ImageInfo.Height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); 
		ilDeleteImages(1, &image);

		LOG("Texture loaded correctly");
		return new Texture(texture_id, ImageInfo.Width, ImageInfo.Height, file_path);
	}
	else
	{
		LOG("DevIL Error");
		ilDeleteImages(1, &image);
		return nullptr;
	}
}
