#pragma once
#include "Module.h"
#include "Globals.h"

#include "../External/FreeType/include/ft2build.h"
#include "../External/FreeType/include/freetype/freetype.h"
#include "../External/FreeType/include/freetype/ftglyph.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

#include "../External/Glew/include/glew.h"

#pragma comment(lib, "Game/External/FreeType/libx86/freetype.lib")

#include <vector>
#include <map>

#define DEFAULT_FONT "./Fonts/Arial.ttf"
#define DEFAULT_SIZE 12

typedef char GLchar;
using namespace std;

struct Character
{
	uint textID;
	float2 size;
	float2 bearing;
	uint advance;
};

struct Font
{
	uint size;
	map<GLchar, Character> AllCharacters;
};

class ModuleFont : public Module
{
public:
	ModuleFont(Application* app, bool start_enabled = true);
	~ModuleFont();

	bool Init();
	bool CleanUp();

	Font* const FontLoader(int size, const char* pathToFont);

	void GenerateTexture(GLuint& texture, FT_Face face);

public:
	vector<Font*> AllFonts;
	Font* actualFont = nullptr;
};
