#pragma once

#include "sharedData/constants.hpp"
#include <freetype/freetype.h>

/*
    TODO:
        - store bitmaps in std::map, create bitmaps when first needed
*/

class TextPrinter
{
public:
    TextPrinter(const float font_size, const float color_rgb,
                const float boldness_text);
    ~TextPrinter();

    void drawText(const float x, const float y, const char *text,
                  const bool background = false,
                  const float *gl_color = GLCOLOR_BLACK) const;

private:
    float drawCharacterReturnWidth(const char character, const float x,
                                   float y) const;
    void flipBitmap(FT_Bitmap *bitmap) const;
    void applyColor(FT_Bitmap *bitmap) const;

    FT_Library ftLibrary;
    FT_Face ftFace;

    const float font_size;
    const float color_rgb;
    const float boldness;
};
