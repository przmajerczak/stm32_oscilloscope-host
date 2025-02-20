#pragma once

#include <freetype/freetype.h>

/*
    TODO:
        - store bitmaps in std::map, create bitmaps when first needed
*/

class TextPrinter
{
public:
    TextPrinter();
    ~TextPrinter();

    void drawText(const float x, const float y, const char *text,
                  const float font_size, const float color_rgb,
                  [[maybe_unused]] const float boldness);

private:
    void flipBitmap(FT_Bitmap *bitmap);
    void applyColor(FT_Bitmap *bitmap, const float color_rgb);

    FT_Library ftLibrary;
    FT_Face ftFace;
};
