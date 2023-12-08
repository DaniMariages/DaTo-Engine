#pragma once
#include "Resource.h"

class ResourceFolder : public Resource
{
public:
	ResourceFolder(const char* assetsFile, const char* libraryFile, const char* name, uint UID);
	ResourceFolder();
	~ResourceFolder();
};
