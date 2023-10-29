#pragma once

#include "Globals.h"
#include "Application.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Mesh.h"
#include <vector>
#include <string>

class LoadFBX
{
public:

    LoadFBX() {}
    ~LoadFBX() {}

	void Load(const char* path) 
	{
		loadModel(path);
	}

    void Draw()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();
    }
    
    void DrawVertex() 
    {
        for (unsigned int m = 0; m < meshes.size(); ++m)
        {
            for (unsigned int j = 0; j < meshes[m].vertices.size(); ++j)
            {
                const Vertex& vertex = meshes[m].vertices[j];
                
                float3 vertexPosition = vertex.Position;
                float3 vertexNormal = vertex.Normal;

                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_LINES);
                glVertex3f(vertexPosition.x, vertexPosition.y, vertexPosition.z);
                glVertex3f(vertexPosition.x + vertexNormal.x, vertexPosition.y + vertexNormal.y, vertexPosition.z + vertexNormal.z);
                glEnd();
            }
        }
    }

    void DrawFaces() 
    {
        for (unsigned int m = 0; m < meshes.size(); ++m)
        {
            for (unsigned int j = 0; j < meshes[m].indices.size(); j += 3)
            {
                unsigned int vertexIndex1 = meshes[m].indices[j];
                unsigned int vertexIndex2 = meshes[m].indices[j + 1];
                unsigned int vertexIndex3 = meshes[m].indices[j + 2];

                float3 faceCenter = (meshes[m].vertices[vertexIndex1].Position +
                    meshes[m].vertices[vertexIndex2].Position +
                    meshes[m].vertices[vertexIndex3].Position) /
                    3.0f;

                float3 faceNormal = CalculateFaceNormal(
                    meshes[m].vertices[vertexIndex1].Position,
                    meshes[m].vertices[vertexIndex2].Position,
                    meshes[m].vertices[vertexIndex3].Position);

                glColor3f(0.0f, 0.0f, 1.0f);
                glBegin(GL_LINES);
                glVertex3f(faceCenter.x, faceCenter.y, faceCenter.z);
                glVertex3f(faceCenter.x + faceNormal.x, faceCenter.y + faceNormal.y, faceCenter.z + faceNormal.z);
                glEnd();
            }
        }
    }

    void GetUniqueModelName(std::vector<LoadFBX>& models)
    {
        int counter = 0;
        if (models.size() > 0)
        {
            for (int i = 0; i < models.size(); i++)
            {
                if (modelName == models[i].modelName)
                {
                    counter++;
                    existingName(modelName, counter);
                }
            }
        }
    }
    
    // model data
    std::vector<Mesh> meshes;
    std::string modelName;
    bool hide = false;

private:

    void loadModel(const char* file_path)
    {
        const aiScene* scene = nullptr;

        if(file_path != nullptr) 
            scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG("Error loading scene: %s", file_path);
            return;
        }
        else
        {
            LOG("Scene loaded succesfully: %s.", file_path);
            modelName = GetModelName(file_path);
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices; 

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            float3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
  
        LOG("Num Vertices: %d.", mesh->mNumVertices);
        LOG("Num Caras: %d.", mesh->mNumFaces);
        
        LOG("Vertices: %d", vertices);
        LOG("Indices: %d", indices);

        return Mesh(vertices, indices);
    }

    float3 CalculateFaceNormal(const float3& vertex1, const float3& vertex2, const float3& vertex3)
    {
        float3 edge1 = vertex2 - vertex1;
        float3 edge2 = vertex3 - vertex1;
        return Cross(edge1, edge2).Normalized();
    }

    std::string GetModelName(const char* file_path)
    {
        std::string getName(file_path);
        size_t lastSeparator = getName.find_last_of("\\");
        
        std::string name;
        if (lastSeparator != std::string::npos)
        {
            name = getName.substr(lastSeparator + 1);
        }
        else
        {
            name = getName;
        }

        LOG("El nombre del modelo es: %s", name.c_str());
        return name;
    }

    std::string existingName(std::string name, int counter)
    {
        std::string uniqueName = name + " (" + std::to_string(counter) + ")";
        std::string newName;

        size_t first = uniqueName.find_first_of("(");

        if (first > 0)
        {
            newName = uniqueName.erase(first - 1);
            newName = uniqueName + " (" + std::to_string(counter) + ")";
        }
        
        modelName = newName;

        return uniqueName;
    }
};