#include "Globals.h"

#include "Application.h"
#include "ModuleFileSystem.h"

#include "ImporterTexture.h"
#include "GameObject.h"
#include "ComponentTexture.h"

//#include "ModuleResource.h"
//#include "ResourceMaterial.h"
//#include "ResourceTexture.h"

#include "../External/Assimp/include/material.h"
#include "../External/Assimp/include/texture.h"

#include "../External/Devil/Include/ilut.h"
#include "../External/Devil/Include/ilu.h"
#include "../External/Devil/Include/il.h"

#pragma comment (lib, "Game/External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Game/External/Devil/libx86/ILU.lib")
#pragma comment (lib, "Game/External/Devil/libx86/ILUT.lib")

void Importer::TextureImporter::InitDevil()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void Importer::TextureImporter::ImportTexture(ResourceTexture* rMaterial, const char* buffer, uint size)
{

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size))
	{
		LOG("Succesfully imported texture");
	}
	else
	{
		LOG("ERROR: Texture could not be loaded");
	}
}

uint Importer::TextureImporter::Save(const Texture* texture, char** buffer)
{
	ilEnable(IL_FILE_OVERWRITE);

	ILuint size;
	ILubyte* ILbuffer;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0) {
		ILbuffer = new ILubyte[size];
		if (ilSaveL(IL_DDS, ILbuffer, size) > 0)
		{
			*buffer = (char*)ILbuffer;
		}
	}
	
	return size;
}

void Importer::TextureImporter::Load(Texture* texture, char* buffer, uint size)
{
	ILuint Il_Tex;

	ilGenImages(1, &Il_Tex);
	ilBindImage(Il_Tex);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size);
	texture->textID = (ilutGLBindTexImage());
	ilDeleteImages(1, &Il_Tex);

}

