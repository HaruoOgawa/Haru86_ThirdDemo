#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace text
{
	TextRenderer::TextRenderer()
	{
		FT_Library library;
		FT_Face face;
		FT_GlyphSlot slot;

		FT_Init_FreeType(&library);
		FT_New_Face(library, "C:\\Windows\\Fonts\\meiryo.ttc", 0, &face);
		FT_Set_Pixel_Sizes(face, 0, 48);
		slot = face->glyph;
	}
	
	TextRenderer::~TextRenderer()
	{

	}
}