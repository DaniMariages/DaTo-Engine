#include "../External/Glew/include/glew.h"
#include "../External/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ResourceMesh.h"
#include "ImporterMesh.h"

#include "Globals.h"

ResourceMesh::ResourceMesh() : Resource(ResourceType::Mesh) {}

ResourceMesh::ResourceMesh(const char* assetsFile, const char* libraryFile, const char* name, uint UID) : Resource(ResourceType::Mesh)
{
    this->name = name;
    this->libraryFile = libraryFile;
    this->assetsFile = assetsFile;
    if (UID != 0) this->UID = UID;
}

ResourceMesh::~ResourceMesh() {}

void ResourceMesh::DrawNormals()
{
    //glBegin(GL_LINES);

    //for (uint i = 0; i < vertices.size(); ++i)																
    //{
    //    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    //    glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
    //    glVertex3f(vertices[i].position.x + vertices[i].normal.x, vertices[i].position.y + vertices[i].normal.y, vertices[i].position.z + vertices[i].normal.z);
    //}

    //glEnd();
}

void ResourceMesh::DrawTexCoords()
{
    /*  glBegin(GL_POINTS);
      glPointSize(100.0f);
      for (uint i = 0; i < vertices.size(); ++i)
      {
          glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
          glVertex3f(vertices[i].position.x + vertices[i].texCoords.x, vertices[i].position.y + vertices[i].texCoords.y, vertices[i].position.z);
      }

      glPointSize(1.0f);
      glEnd();*/
}

void ResourceMesh::SetUpBuffers(ResourceMesh* Mesh)
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

