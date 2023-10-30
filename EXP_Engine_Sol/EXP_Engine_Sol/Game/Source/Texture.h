#pragma once
#include "Globals.h"
#include <vector>
#include <string>

class Texture
{
	unsigned int id;
	std::string type;
	std::string path;

	void LoadTexture(std::string path);
	void SetTexture();
};
