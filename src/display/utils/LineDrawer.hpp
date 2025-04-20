#pragma once

#include "display/utils/TextPrinter.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/constants.hpp"

class LineDrawer
{
public:
    LineDrawer(const DynamicData &dynamicData) : dynamicData(dynamicData) {}

    void drawTriggerIndicator(const float x, const float y);
    void drawGrid(const int numOfVerticalLayers, const int numOfHorizontalLayers);
    void drawDisplayAreaBorder();
    void drawVerticalMeasurement();
    void drawHorizontalMeasurement();

private:
    void drawHorizontalLine(const float y, const float boldness,
                            const bool stipple_line = false, const float x1 = 0,
                            const float x2 = X_DISPLAY_RESOLUTION);
    void drawVerticalLine(const float x, const float boldness,
                          const bool stipple_line = false, const float y1 = 0,
                          const float y2 = Y_DISPLAY_RESOLUTION);
    void drawHorizontalLineWithLabels(const float y, const char *unit_label,
                                      const TextPrinter &textPrinter,
                                      const float boldness_line = NEUTRAL,
                                      const bool stipple_line = false);
    void drawVerticalLineWithLabels(const float x, const char *value_label,
                                    const char *unit_label,
                                    const TextPrinter &textPrinter,
                                    const float boldness_line = NEUTRAL,
                                    const bool stipple_line = false);
    void drawHorizontalGrid(const int numOfHorizontalLayers);
    void drawVerticalGrid(const int numOfVerticalLayers);

    TextPrinter textPrinterForTriggerIndicator{
        FONT_SIZE_MEDIUM, MONOCHROME_TEXT_COLOR_WHITE, NEUTRAL};
    TextPrinter textPrinterForGrid{FONT_SIZE_SMALL,
                                   MONOCHROME_TEXT_COLOR_DARK_GRAY, NEUTRAL};
    TextPrinter textPrinterForMeasurements{FONT_SIZE_MEDIUM,
                                           MONOCHROME_TEXT_COLOR_WHITE, NEUTRAL};

    const DynamicData &dynamicData;
};
