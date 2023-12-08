#pragma once
#include "Globals.h"
#include "Color.h"
#include "Resource.h"

class ResourceTexture : public Resource
{
public:

	ResourceTexture(const char* assetsFile, const char* libraryFile, const char* name, int height, int width, uint UID);
	ResourceTexture(uint ID, std::string path);
	ResourceTexture();
	~ResourceTexture();

public:

	uint id = 0;
	int height = 0; 
	int width = 0;
	std::string path = "Default";

};