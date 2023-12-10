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

	//Assign a unique Name to the new game Object
	name = GetUniqueName(name);

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

		if (newGameObject->children.size() > 0) //If game Object has childrens, add the textures to them too
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

void ModuleImport::LoadMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		if (GO == typeOfGO::CHILD_OF_SCENE) 
			GetSceneInfo(scene->mRootNode, scene, file_path, nullptr);

		if (GO == typeOfGO::CHILD_OF_OBJECT)
		{
			scene->mRootNode->mName = name; //change the root name to the game object name
			GetSceneInfo(scene->mRootNode, scene, file_path, App->scene->gameObjectSelected);
		}
			
		LOG("Scene loaded correctly");
		aiReleaseImport(scene);
	}
	else LOG("Error loading scene % s", file_path);
}

void ModuleImport::GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	//Needed to know where add mesh
	GameObject* tempObject{};

	// MYTODO: BakerHouse initial transform its bad, so do this when load Street Environment
	
	//Get the transform of the node
	aiVector3D position, scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, position);

	//Data to fill with the transform
	goTransform tempTrans;

	//Set position
	tempTrans.position.x = position.x;
	tempTrans.position.y = position.y;
	tempTrans.position.z = position.z;

	//Set rotation
	tempTrans.rotation.x = rotation.GetEuler().x;
	tempTrans.rotation.y = rotation.GetEuler().y;
	tempTrans.rotation.z = rotation.GetEuler().z;

	//Set scale
	tempTrans.scale.x = scale.x;
	tempTrans.scale.y = scale.y;
	tempTrans.scale.z = scale.z;

	//If gameObject is null, means that is a scene child 
	if (gameObject == nullptr)
	{
		//Create a new game object that is child of scene
		tempObject = App->scene->CreateGameObject(name, App->scene->rootGameObject);

		tempObject->transform->SetPosition(tempTrans.position);
		tempObject->transform->SetEulerRotation(tempTrans.rotation);
		tempObject->transform->SetScale(tempTrans.scale);

		newGameObject = tempObject;
	}
	//else means that is a gameObject child
	else
	{
		tempObject = App->scene->CreateGameObject(node->mName.C_Str(), gameObject);
		tempObject->transform->SetPosition(tempTrans.position);
		tempObject->transform->SetEulerRotation(tempTrans.rotation);
		tempObject->transform->SetScale(tempTrans.scale);

	}

	//Process all the meshes of the node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], file_path, tempObject);
	}

	//If node has children, process them
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
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
	compMesh->InitBoundingBoxes(myMesh);

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

//MYTODO: This should be done with Resource
void ModuleImport::SaveGameObject(GameObject* gameObject)
{
	JsonConfig jsonFile;
	ArrayConfig jsonArray = jsonFile.SetArray("3D Models");

	JsonConfig& node = jsonArray.AddNode();

	node.SetString("Name", gameObject->Name);
	node.SetNumber("ID", gameObject->ID);
}