#include "Application.h"
#include "ModuleTexture.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>

#pragma comment(lib, "Game/External/DevIL/libx86/DevIL.lib")
#pragma comment(lib, "Game/External/DevIL/libx86/ILU.lib")
#pragma comment(lib, "Game/External/DevIL/libx86/ILUT.lib")

ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleTexture::~ModuleTexture() {}

bool ModuleTexture::Start()
{
    ilInit();
    ilutRenderer(ILUT_OPENGL);

    return true;
}

bool ModuleTexture::CleanUp()
{
    ilShutDown();
    return true;
}

Texture* ModuleTexture::LoadTexture(const char* file_path)
{
    ILuint image;
    GLboolean imageLoaded;

    ilGenImages(1, &image);
    ilBindImage(image);

    if (ilLoadImage(file_path))
    {
        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);

        GLuint texture_id;
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
        return new Texture(texture_id, ImageInfo.Width, ImageInfo.Height);
    }
    else
    {
        LOG("DevIL Error");
        ilDeleteImages(1, &image);
        return nullptr;
    }
}


update_status ModuleTexture::Update(float dt) {return UPDATE_CONTINUE;}