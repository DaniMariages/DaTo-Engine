#pragma once
#include "Module.h"
#include "Globals.h"

#include "../External/MathGeoLib/include/MathGeolib.h"

#include "../External/DevIL/include/il.h"
#include "../External/DevIL/include/ilu.h"
#include "../External/DevIL/include/ilut.h"

struct Texture
{
	GLuint textID;
	uint width;
	uint height;

	Texture(GLuint id, uint _width, uint _height)
	{
		textID = id;
		width = _width;
		height = _height;
	}
};

class ModuleTexture : public Module
{
public:
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	Texture* LoadTexture(const char* file_path);
	std::string path;
};