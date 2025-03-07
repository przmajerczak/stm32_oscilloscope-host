#pragma once

#include "display/utils/TextPrinter.hpp"
#include "sharedData/constants.hpp"
#include "sharedData/DynamicData.hpp"
#include "utils.hpp"
#include <string>

class LineDrawer
{
public:
    LineDrawer(const DynamicData &dynamicData) : dynamicData(dynamicData) {}

    void drawTriggerIndicator(const int x, const int y);
    void drawGrid(const int numOfVerticalLayers,
                  const int numOfHorizontalLayers);
    void drawDisplayAreaBorder();

private:
    void drawHorizontalLine(const int y, const float boldness,
                            const bool stipple_line = false);
    void drawVerticalLine(const int x, const float boldness,
                          const bool stipple_line = false);
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

    TextPrinter textPrinterForTriggerIndicator{FONT_SIZE_LARGE, COLOR_RGB_WHITE, NEUTRAL};
    TextPrinter textPrinterForGrid{FONT_SIZE_SMALL, COLOR_RGB_DARK_GRAY, NEUTRAL};

    const DynamicData &dynamicData;
};
