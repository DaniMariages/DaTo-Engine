#include "ModuleFonts.h"
#include "../External/FreeType/include/ft2build.h"
#include "../External/FreeType/include/freetype/freetype.h"
#include "../External/FreeType/include/freetype/ftglyph.h"
#include "../External/Glew/include/glew.h"

#pragma comment(lib, "FreeType/libx86/freetype.lib")

bool ModuleFonts::Init()
{
	LOG("Init True Type Font library");
	bool ret = true;

	return ret;
}

bool ModuleFonts::Start()
{
	return true;
}

bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type library");
	return true;
}

Font* const ModuleFonts::LoadFont(const char* path, int size)
{
	Font* font = new Font;

	font->size = size;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		LOG("ERROR::FREETYPE: Could not init FreeType Library");

	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
		LOG("ERROR::FREETYPE: Failed to load font");

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (int c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}

		// Generate texture
		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			float2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		font->characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return font;
}

bool ModuleFonts::CalcSize(const char* text, int& width, int& height) const
{
	bool ret = false;


	return ret;
}
