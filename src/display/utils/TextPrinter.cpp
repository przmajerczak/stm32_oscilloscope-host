#include "TextPrinter.hpp"

#include "sharedData/constants.hpp"
#include <GL/glut.h>

TextPrinter::TextPrinter()
{
    FT_Init_FreeType(&ftLibrary);
    FT_New_Face(ftLibrary, FONT_PATH, 0, &ftFace);
}

TextPrinter::~TextPrinter()
{
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);
}

void TextPrinter::drawText(const float x, const float y, const char *text,
                           const float font_size, const float color_rgb,
                           const float boldness)
{
    if (text != nullptr)
    {
        FT_Set_Pixel_Sizes(ftFace, font_size, font_size);

        glDisable(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glColor3f(color_rgb, color_rgb, color_rgb);

        float character_x{x};
        for (const char *character = text; *character; ++character)
        {
            character_x += drawCharacterReturnWidth(*character, character_x, y, color_rgb);
        }
    }
}

float TextPrinter::drawCharacterReturnWidth(const char character, const float x,
                                            const float y,
                                            const float color_rgb)
{
    FT_Load_Char(ftFace, character, FT_LOAD_RENDER);

    FT_GlyphSlot glyph = ftFace->glyph;
    FT_Bitmap *bitmap = &glyph->bitmap;

    flipBitmap(bitmap);
    applyColor(bitmap, color_rgb);

    glRasterPos2f(x + glyph->bitmap_left, y);
    glDrawPixels(bitmap->width, bitmap->rows, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 bitmap->buffer);

    return bitmap->width;
}

void TextPrinter::flipBitmap(FT_Bitmap *bitmap)
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

void TextPrinter::applyColor(FT_Bitmap *bitmap, const float color_rgb)
{
    const int width = bitmap->width;
    const int height = bitmap->rows;

    for (int i = 0; i < width * height; ++i)
    {
        bitmap->buffer[i] *= color_rgb;
    }
}
