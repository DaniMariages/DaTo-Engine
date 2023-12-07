#include "Resource.h"

Resource::Resource()
{
	UID = randomNum.GenerateRandomInt();
}

Resource::Resource(ResourceType type) : type(type)
{
	UID = randomNum.GenerateRandomInt();
}

Resource::Resource(ResourceType _type, const char* _assetsFile, const char* _name, uint ID)	
{
	type = _type;
	name = _name;
	assetsFile = _assetsFile;
	UID = ID;

	UID == 0 ? randomNum.GenerateRandomInt() : ID;
}

Resource::~Resource()
{

}