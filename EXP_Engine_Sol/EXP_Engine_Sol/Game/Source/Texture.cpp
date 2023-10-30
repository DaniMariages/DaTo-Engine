#include "Texture.h"
#include <string>
#include <vector>

#include "../External/DevIL/include/il.h"
#include "../External/DevIL/include/ilu.h"
#include "../External/DevIL/include/ilut.h"

#pragma comment (lib, "Game/External/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Game/External/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Game/External/DevIL/libx86/ILUT.lib")

void Texture::LoadTexture(std::string path) 
{
	ilInit();

}