#pragma once

#include "display/utils/TextPrinter.hpp"
#include "sharedData/DynamicData.hpp"
#include "sharedData/constants.hpp"

class LineDrawer
{
public:
    LineDrawer(const DynamicData &dynamicData) : dynamicData(dynamicData) {}

    void drawTriggerIndicator(const int x, const int y);
    void drawGrid(const int numOfVerticalLayers, const int numOfHorizontalLayers);
    void drawDisplayAreaBorder();
    void drawVerticalMeasurement();
    void drawHorizontalMeasurement();

private:
    void drawHorizontalLine(const int y, const float boldness,
                            const bool stipple_line = false, const int x1 = 0,
                            const int x2 = X_DISPLAY_RESOLUTION);
    void drawVerticalLine(const int x, const float boldness,
                          const bool stipple_line = false, const int y1 = 0,
                          const int y2 = Y_DISPLAY_RESOLUTION);
    void drawHorizontalLineWithLabels(const int y, const char *unit_label,
                                      const TextPrinter &textPrinter,
                                      const float boldness_line = NEUTRAL,
                                      const bool stipple_line = false);
    void drawVerticalLineWithLabels(const int x, const char *value_label,
                                    const char *unit_label,
                                    const TextPrinter &textPrinter,
                                    const float boldness_line = NEUTRAL,
                                    const bool stipple_line = false);
    void drawHorizontalGrid(const int numOfHorizontalLayers);
    void drawVerticalGrid(const int numOfVerticalLayers);

    TextPrinter textPrinterForTriggerIndicator{FONT_SIZE_MEDIUM, MONOCHROME_TEXT_COLOR_WHITE,
                                               NEUTRAL};
    TextPrinter textPrinterForGrid{FONT_SIZE_SMALL, MONOCHROME_TEXT_COLOR_DARK_GRAY, NEUTRAL};
    TextPrinter textPrinterForMeasurements{FONT_SIZE_MEDIUM, MONOCHROME_TEXT_COLOR_WHITE,
                                           NEUTRAL};

    const DynamicData &dynamicData;
};
