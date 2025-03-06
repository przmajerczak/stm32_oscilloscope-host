#pragma once

#include <gtk/gtk.h>

#include "CallbackData.hpp"
#include "sharedData/DynamicData.hpp"

class DebugDataControls
{
public:
    void prepare(DynamicData &dynamicData);
    GtkWidget *getDebugDataControlsContainer();

private:
    void prepareTimemarkerDataForDataRetrievalDataLabel(DynamicData &dynamicData);
    void prepareTimemarkerDataForDataAnalysisDataLabel(DynamicData &dynamicData);
    void prepareTimemarkerDataForFrameDisplayDataLabel(DynamicData &dynamicData);
    void prepareNanosecondsPerSampleLabel(DynamicData &dynamicData);

    GtkWidget *timemarkerDataForDataRetrievalDataLabel;
    GtkWidget *timemarkerDataForDataAnalysisDataLabel;
    GtkWidget *timemarkerDataForFrameDisplayDataLabel;
    GtkWidget *nanosecondsPerSampleLabel;

    CallbackData<DynamicData> callbackDataForDataRetrievalData;
    CallbackData<DynamicData> callbackDataForDataAnalysisData;
    CallbackData<DynamicData> callbackDataForFrameDisplayData;
    CallbackData<DynamicData> callbackDataForNanosecondsPerSample;
};
