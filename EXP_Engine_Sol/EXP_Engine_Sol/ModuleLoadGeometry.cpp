#include "Globals.h"
#include "Application.h"
#include "ModuleLoadGeometry.h"

ModuleLoadGeometry::ModuleLoadGeometry(Application* app, bool start_enabled) : Module(app, start_enabled){}
ModuleLoadGeometry::~ModuleLoadGeometry() {}

ModuleLoadGeometry::Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

bool ModuleLoadGeometry::Init() 
{
	LOG("Creating Load Geometry context");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleLoadGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

update_status ModuleLoadGeometry::PreUpdate(float dt) 
{
	LoadFile();
	LoadMeshes();
	return UPDATE_CONTINUE;
}

update_status ModuleLoadGeometry::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleLoadGeometry::LoadFile() 
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Loading FBX from Path: %s", file_path);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i <= scene->mNumMeshes; ++i) 
		{
			scene->mMeshes[i];
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", file_path);
}

void ModuleLoadGeometry::LoadMeshes()
{
	
}

void ModuleLoadGeometry::Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

