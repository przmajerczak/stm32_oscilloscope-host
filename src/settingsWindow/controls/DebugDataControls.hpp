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

    GtkWidget *timemarkerDataForDataRetrievalDataLabel;
    GtkWidget *timemarkerDataForDataAnalysisDataLabel;
    GtkWidget *timemarkerDataForFrameDisplayDataLabel;

    CallbackData<DynamicData> callbackDataForDataRetrievalData;
    CallbackData<DynamicData> callbackDataForDataAnalysisData;
    CallbackData<DynamicData> callbackDataForFrameDisplayData;
};
