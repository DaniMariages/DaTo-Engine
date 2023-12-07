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

void ModuleImport::ReadFile(const char* filePath)
{
	LOG("Reading file, path: %s.", filePath);

	name = GetName(filePath);
	path = filePath;
	typeFile extension = ReadExtension(name);

	//Assign a unique Name to the new game Object
	name = GetUniqueName(name);

	switch (extension) {
	case typeFile::MODEL:
		LOG("START LOADING MODEL");

		//The new game object is children of some game object
		if (App->scene->gameObjectSelected != nullptr && App->scene->gameObjectSelected != App->scene->rootGameObject)
		{
			GO = typeOfGO::CHILD_OF_OBJECT;
			LoadMesh(filePath);
			LOG("MODEL LOADED");
		}

		//The new game object is no child, so is child of scene
		else
		{
			GO = typeOfGO::CHILD_OF_SCENE;
			LoadMesh(filePath);
			LOG("MODEL LOADED");
		}

		break;

	case typeFile::TEXTURE:
		LOG("START LOADING TEXTURE");

		if (newGameObject->children.size() > 0) //If game Object has childrens, add the textures to them too
		{
			for (unsigned int i = 0; i < newGameObject->children.size(); i++)
			{
				ComponentTexture* tempCompTex = new ComponentTexture(newGameObject->children[i]);
				tempCompTex->SetTexture(LoadTexture(filePath));
				LOG("Texture of: %s is loaded", newGameObject->children[i]->Name.c_str());
				newGameObject->children[i]->AddComponent(tempCompTex);
			}
		}
		else
		{
			ComponentTexture* tempCompTex = new ComponentTexture(newGameObject);
			tempCompTex->SetTexture(LoadTexture(filePath));
			LOG("Texture of: %s is loaded", newGameObject->Name.c_str());
			newGameObject->AddComponent(tempCompTex);
		}
		
		break;
	}
}

std::string ModuleImport::GetName(const char* filePath)
{
	std::string getName(filePath);
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

std::string ModuleImport::GetUniqueName(std::string Name)
{
	//Check if a Game Object with same name exists
	bool exists = false;
	int counter = 0;
	if (App->scene->gameObjects.size() > 0)
	{
		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			if (Name == App->scene->gameObjects[i]->Name)	//If the name exists, add 1 to counter
			{
				counter++;
				Name = ReName(Name, counter);
			}
		}
		return Name;
	}
	else return Name;
}

std::string ModuleImport::ReName(std::string Name, int counter)
{
	std::string uniqueName = Name + " (" + std::to_string(counter) + ")";
	std::string newName;

	size_t first = uniqueName.find_first_of("(");

	newName = uniqueName.erase(first - 1);
	newName = uniqueName + " (" + std::to_string(counter) + ")";
	
	return newName;
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
		LOG("The file dropped is a: Model");
	}
	else if (extension == "png")
	{
		typeExtension = typeFile::TEXTURE;
		LOG("The file dropped is a: Texture");
	}
	else if (extension == "dds") {
		typeExtension = typeFile::TEXTURE;
		LOG("The file dropped is a: Texture");
	}

	LOG("The file extension is: %s", extension.c_str());
	return typeExtension;
}

//------------------------- MESHES -------------------------

void ModuleImport::LoadMesh(const char* filePath)
{
	const aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		if (GO == typeOfGO::CHILD_OF_SCENE) 
			GetSceneInfo(scene->mRootNode, scene, filePath, nullptr);

		if (GO == typeOfGO::CHILD_OF_OBJECT)
		{
			scene->mRootNode->mName = name; //change the root name to the game object name
			GetSceneInfo(scene->mRootNode, scene, filePath, App->scene->gameObjectSelected);
		}
			
		//ComponentTransform* compTrans = new ComponentTransform(newGameObject);	//PROVISIONAL
		//newGameObject->AddComponent(compTrans);

		LOG("Scene loaded correctly");
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene % s", filePath);
}

void ModuleImport::GetSceneInfo(aiNode* node, const aiScene* scene, const char* filePath, GameObject* gameObject)
{
	GameObject* tempObject{}; //Needed to know where add mesh
	
	if (gameObject == nullptr)
	{
		tempObject = App->scene->CreateGameObject(name, App->scene->rootGameObject);
		newGameObject = tempObject;
	}
	else tempObject = App->scene->CreateGameObject(node->mName.C_Str(), gameObject);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], filePath, tempObject);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		GetSceneInfo(node->mChildren[i], scene, filePath, tempObject);
	}
}

mesh ModuleImport::ProcessMesh(aiMesh* Mesh, const char* filePath, GameObject* gameObject)
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
	compMesh->SetPath(std::string(filePath));
	compMesh->SetMesh(myMesh);
	compMesh->InitBoundingBoxes(myMesh);

	gameObject->AddComponent(compMesh);
	SetUpBuffers(myMesh);
	meshes.push_back(*myMesh);


	return *myMesh;
}

void ModuleImport::SetUpBuffers(const mesh* Mesh)
{
	glGenBuffers(1, &(GLuint)Mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Mesh->vertices.size(), &Mesh->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &(GLuint)Mesh->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Mesh->indices.size(), &Mesh->indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//------------------------- TEXTURES -------------------------

Texture* ModuleImport::LoadTexture(const char* filePath)
{
	ILenum image;
	GLboolean imageLoaded;

	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoadImage(filePath))
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
		newTexture->path = filePath;

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
