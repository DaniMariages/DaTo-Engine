#include "ModuleFont.h"
#include "Application.h"

ModuleFont::ModuleFont(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	actualFont = nullptr;
}

ModuleFont::~ModuleFont()
{
}

bool ModuleFont::Init()
{
	bool ret = true;

	LOG("Init Module Font");

	return ret;
}

bool ModuleFont::CleanUp()
{
	bool ret = true;

	return ret;
}

Font* const ModuleFont::FontLoader(int size, const char* pathToFont)
{
	Font* font = new Font;

	font->size = size;

	FT_Library ft_Lib;

	bool error;

	error = FT_Init_FreeType(&ft_Lib);

	if (error)
	{
		LOG("Free Type couldn't Init!");
	}

	FT_Face ft_Face;

	error = FT_New_Face(ft_Lib, pathToFont, 0, &ft_Face);

	if (error)
	{
		LOG("Free Type couldn't New Face Font!");
	}

	FT_Set_Pixel_Sizes(ft_Face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 0; i < 128; i++)
	{
		error = FT_Load_Char(ft_Face, i, FT_LOAD_RENDER);

		if (error)
		{
			LOG("Free Type couldn't load Char!");
			continue;
		}

		GLuint texture = 0;

		GenerateTexture(texture, ft_Face);

		Character character;
		character.textID = texture;
		character.size = float2(ft_Face->glyph->bitmap.width, ft_Face->glyph->bitmap.rows);
		character.bearing = float2(ft_Face->glyph->bitmap_left, ft_Face->glyph->bitmap_top);
		character.advance = ft_Face->glyph->advance.x;

		font->AllCharacters.insert(pair<GLchar, Character>(i, character));
	}

	return font;
}

void ModuleFont::GenerateTexture(GLuint& texture, FT_Face face)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}