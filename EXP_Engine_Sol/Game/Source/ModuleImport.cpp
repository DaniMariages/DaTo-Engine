#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleImport.h"
#include "ModuleScene.h"

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

		//The new game object is children of some game object
		if (App->scene->gameObjectSelected != nullptr && App->scene->gameObjectSelected != App->scene->rootGameObject)
		{
			GO = typeOfGO::CHILD_OF_OBJECT;
			LoadMesh(file_path);
			LOG("MODEL LOADED");
		}

		//The new game object is no child, so is child of scene
		else
		{
			GO = typeOfGO::CHILD_OF_SCENE;
			LoadMesh(file_path);
			LOG("MODEL LOADED");
		}

		break;

	case typeFile::TEXTURE:
		LOG("START LOADING TEXTURE");

		if (newGameObject->children.size() > 0)
		{
			for (unsigned int i = 0; i < newGameObject->children.size(); i++)
			{
				ComponentTexture* tempCompTex = new ComponentTexture(newGameObject->children[i]);
				tempCompTex->SetTexture(LoadTexture(file_path));
				LOG("Texture of: %s is loaded", newGameObject->children[i]->Name.c_str());
				newGameObject->children[i]->AddComponent(tempCompTex);
			}
		}
		else
		{
			ComponentTexture* tempCompTex = new ComponentTexture(newGameObject);
			tempCompTex->SetTexture(LoadTexture(file_path));
			LOG("Texture of: %s is loaded", newGameObject->Name.c_str());
			newGameObject->AddComponent(tempCompTex);
		}
		
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
	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		if (GO == typeOfGO::CHILD_OF_SCENE) 
			GetSceneInfo(scene->mRootNode, scene, file_path, nullptr);

		if (GO == typeOfGO::CHILD_OF_OBJECT) 
			GetSceneInfo(scene->mRootNode, scene, file_path, App->scene->gameObjectSelected);

		ComponentTransform* compTrans = new ComponentTransform(newGameObject);	//PROVISIONAL
		newGameObject->AddComponent(compTrans);

		LOG("Scene loaded correctly");
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene % s", file_path);
}

void ModuleImport::GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	GameObject* tempObject{}; //Needed to know where add mesh

	if (gameObject == nullptr)
	{
		tempObject = App->scene->CreateGameObject(name, App->scene->rootGameObject);
		newGameObject = tempObject;
	}
	else if (gameObject != nullptr) 
		tempObject = App->scene->CreateGameObject(node->mName.C_Str(), gameObject);
	else if (gameObject == App->scene->gameObjectSelected && GO == typeOfGO::CHILD_OF_OBJECT)
		tempObject = App->scene->CreateGameObject(name, gameObject);
	
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], file_path, tempObject);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LOG("I FOUND THIS: %s", node->mChildren[i]->mName.C_Str());
		GetSceneInfo(node->mChildren[i], scene, file_path, tempObject);
	}
}

mesh ModuleImport::ProcessMesh(aiMesh* Mesh, const char* file_path, GameObject* gameObject)
{
	mesh* myMesh = new mesh();
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

	gameObject->AddComponent(compMesh);
	App->renderer3D->SetUpBuffers(myMesh);
	meshes.push_back(*myMesh);

	return *myMesh;
}


Texture* ModuleImport::LoadTexture(const char* file_path)
{
	ILenum image;
	GLboolean imageLoaded;

	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoadImage(file_path))
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ILinfo ImageInfo;
		GLuint texture_id;

		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		iluGetImageInfo(&ImageInfo);
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (ImageInfo.Origin == IL_ORIGIN_LOWER_LEFT)
		{
			iluFlipImage();
		}

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ImageInfo.Width, ImageInfo.Height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		ilDeleteImages(1, &image);

		Texture* newTexture = new Texture();
		newTexture->height = ImageInfo.Height;
		newTexture->width = ImageInfo.Width;
		newTexture->textID = texture_id;
		newTexture->path = file_path;

		LOG("Texture loaded correctly");
		return newTexture;
	}
	else
	{
		LOG("DevIL Error");
		ilDeleteImages(1, &image);
		return nullptr;
	}
}
