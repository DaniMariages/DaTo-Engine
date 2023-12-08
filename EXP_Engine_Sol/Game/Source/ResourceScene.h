#pragma once
#include "Globals.h"
#include "Resource.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/Quat.h"

class GameObject;
struct ModelContainer
{
	ModelContainer(uint ID, const char* name = "", const float3& position = float3::zero, const float3& scale = float3::one, const Quat& rotation = Quat::identity, uint parentID = 0)
		: name(name),
		ID(ID),
		parentID(parentID),
		materialID(-1),
		meshID(-1),
		position(position),
		scale(scale),
		rotation(rotation)
	{}


	std::string name;
	uint ID;
	uint parentID;
	int meshID;
	int materialID;
	float3 position;
	float3 scale;
	Quat rotation;
};


class ResourceScene : public Resource
{
public:
	ResourceScene();
	ResourceScene(const char* assetsFile, const char* libraryFile, const char* name, uint UID);
	~ResourceScene();

public:
	std::vector<ModelContainer> models;
};
