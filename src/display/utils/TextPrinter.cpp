#include "TextPrinter.hpp"

#include "sharedData/constants.hpp"
#include <GL/glew.h>

TextPrinter::TextPrinter(const float font_size, const float color_rgb, const float boldness) : font_size(font_size), color_rgb(color_rgb), boldness(boldness)
{
    FT_Init_FreeType(&ftLibrary);
    FT_New_Face(ftLibrary, FONT_PATH, 0, &ftFace);
    FT_Set_Pixel_Sizes(ftFace, font_size, font_size);
}

TextPrinter::~TextPrinter()
{
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);
}

void TextPrinter::drawText(const float x, const float y, const char *text) const
{
    if (text != nullptr)
    {
        glDisable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        float character_x{x};
        for (const char *character = text; *character; ++character)
        {
            character_x += drawCharacterReturnWidth(*character, character_x, y);
        }
    }
}

float TextPrinter::drawCharacterReturnWidth(const char character, const float x,
                                            float y) const
{
    FT_Load_Char(ftFace, character, FT_LOAD_RENDER);

    FT_GlyphSlot glyph = ftFace->glyph;
    FT_Bitmap *bitmap = &glyph->bitmap;

    flipBitmap(bitmap);
    applyColor(bitmap);

    if (character == '-')
    {
        y += font_size / 4; // TODO: make this workaround more elegant
    }

    glRasterPos2f(x + glyph->bitmap_left, y);
    glDrawPixels(bitmap->width, bitmap->rows, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 bitmap->buffer);

    return bitmap->width;
}

void TextPrinter::flipBitmap(FT_Bitmap *bitmap) const
{
    const int width = bitmap->width;
    const int height = bitmap->rows;
    unsigned char tmp_row[width];

    for (int row = 0; row < height / 2; ++row)
    {
        const int upper_row_index{row * width};
        const int lower_row_index{(height - 1 - row) * width};

        memcpy(tmp_row, &bitmap->buffer[upper_row_index], width);
        memcpy(&bitmap->buffer[upper_row_index], &bitmap->buffer[lower_row_index],
               width);
        memcpy(&bitmap->buffer[lower_row_index], tmp_row, width);
    }
}

void TextPrinter::applyColor(FT_Bitmap *bitmap) const
{
    const int width = bitmap->width;
    const int height = bitmap->rows;

    for (int i = 0; i < width * height; ++i)
    {
        bitmap->buffer[i] *= color_rgb;
    }
}
