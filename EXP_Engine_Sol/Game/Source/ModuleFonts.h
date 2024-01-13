#pragma once
#include "Module.h"
#include "Globals.h"

#include "../External/MathGeoLib/include/MathGeoLib.h"
#include <vector>
#include <map>

#define DEFAULT_FONT "../Fonts/Arial.ttf"

typedef char GLchar;

struct Character 
{
	uint    TextureID;	// ID of character
	float2	Size;		// Size of character
	float2	Bearing;	// Offset from baseline to left/top of character
	uint    Advance;	// Offset to advance to next character
};

struct Font {
	std::map<GLchar, Character> characters;
	int size = 12;
};

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Init();
	bool Start();
	bool CleanUp();

	Font* const LoadFont(const char* path, int size = 12);
	bool CalcSize(const char* text, int& width, int& height) const;

public: 
	std::vector<Font*> fonts;
	Font* defaultFont = nullptr;
};
