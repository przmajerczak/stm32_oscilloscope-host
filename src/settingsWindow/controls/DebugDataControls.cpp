#include "DebugDataControls.hpp"

#include "sharedData/constants.hpp"
#include <sstream>

gboolean
timemarkerDataForDataRetrievalDataLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *timemarkerDataForDataRetrievalDataLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(timemarkerDataForDataRetrievalDataLabel);
    std::stringstream labelContent;

    labelContent
        << "Average total data retrieval and decoding duration: "
        << dynamicData->timemarkersData.totalDataRetrievalAndDecodingDuration
               .getAverageDuration_us()
        << " us. " << std::endl
        << "("
        << dynamicData->timemarkersData.singleFrameDataRetrievalDuration
               .getAverageDuration_us()
        << " of that is single frame data retrieval duration.)" << std::endl;

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean
timemarkerDataForDataAnalysisDataLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *timemarkerDataForDataAnalysisDataLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(timemarkerDataForDataAnalysisDataLabel);
    std::stringstream labelContent;

    labelContent << "Average total data analyzing duration: "
                 << dynamicData->timemarkersData.totalDataAnalyzeDuration
                        .getAverageDuration_us()
                 << " us." << std::endl;

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean
timemarkerDataForFrameDisplayDataLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *timemarkerDataForFrameDisplayDataLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(timemarkerDataForFrameDisplayDataLabel);
    std::stringstream labelContent;

    labelContent
        << "Average total frame display duration: "
        << dynamicData->timemarkersData.totalFrameDisplayDuration
               .getAverageDuration_us()
        << " us. " << std::endl
        << "("
        << dynamicData->timemarkersData.customNonLibraryFrameDisplayDuration
               .getAverageDuration_us()
        << " of that is \"our own\" display duration, the rest is library calls.)";

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean
nanosecondsPerSampleLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *nanosecondsPerSampleLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(nanosecondsPerSampleLabel);
    std::stringstream labelContent;

    labelContent
        << "Nanoseconds per sample: "
        << dynamicData->nanoseconds_per_sample
        << " ns";

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

void DebugDataControls::prepare(DynamicData &dynamicData)
{
    prepareTimemarkerDataForDataRetrievalDataLabel(dynamicData);
    prepareTimemarkerDataForDataAnalysisDataLabel(dynamicData);
    prepareTimemarkerDataForFrameDisplayDataLabel(dynamicData);
    prepareNanosecondsPerSampleLabel(dynamicData);
}

void DebugDataControls::prepareTimemarkerDataForDataRetrievalDataLabel(
    DynamicData &dynamicData)
{
    timemarkerDataForDataRetrievalDataLabel =
        gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(timemarkerDataForDataRetrievalDataLabel, TRUE);

    callbackDataForDataRetrievalData.widget =
        timemarkerDataForDataRetrievalDataLabel;
    callbackDataForDataRetrievalData.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS,
                  timemarkerDataForDataRetrievalDataLabelTimeoutAction,
                  &callbackDataForDataRetrievalData);
}

void DebugDataControls::prepareTimemarkerDataForDataAnalysisDataLabel(
    DynamicData &dynamicData)
{
    timemarkerDataForDataAnalysisDataLabel =
        gtk_label_new("Minimal voltage: --- mV");
    gtk_widget_set_hexpand(timemarkerDataForDataAnalysisDataLabel, TRUE);

    callbackDataForDataAnalysisData.widget =
        timemarkerDataForDataAnalysisDataLabel;
    callbackDataForDataAnalysisData.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS,
                  timemarkerDataForDataAnalysisDataLabelTimeoutAction,
                  &callbackDataForDataAnalysisData);
}

void DebugDataControls::prepareTimemarkerDataForFrameDisplayDataLabel(
    DynamicData &dynamicData)
{
    timemarkerDataForFrameDisplayDataLabel =
        gtk_label_new("Maximal voltage: --- mV");
    gtk_widget_set_hexpand(timemarkerDataForFrameDisplayDataLabel, TRUE);

    callbackDataForFrameDisplayData.widget =
        timemarkerDataForFrameDisplayDataLabel;
    callbackDataForFrameDisplayData.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS,
                  timemarkerDataForFrameDisplayDataLabelTimeoutAction,
                  &callbackDataForFrameDisplayData);
}

void DebugDataControls::prepareNanosecondsPerSampleLabel(
    DynamicData &dynamicData)
{
    nanosecondsPerSampleLabel =
        gtk_label_new("Nanoseconds per sample: --- ns");
    gtk_widget_set_hexpand(nanosecondsPerSampleLabel, TRUE);

    callbackDataForNanosecondsPerSample.widget =
        nanosecondsPerSampleLabel;
    callbackDataForNanosecondsPerSample.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS,
                  nanosecondsPerSampleLabelTimeoutAction,
                  &callbackDataForNanosecondsPerSample);
}

GtkWidget *DebugDataControls::getDebugDataControlsContainer()
{
    GtkWidget *debugDataControlsExpander = gtk_expander_new("Debug");
    gtk_expander_set_expanded(GTK_EXPANDER(debugDataControlsExpander), FALSE);

    constexpr int spacing{0};
    GtkWidget *measurementsVerticalBox =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       timemarkerDataForDataRetrievalDataLabel, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       timemarkerDataForDataAnalysisDataLabel, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       timemarkerDataForFrameDisplayDataLabel, FALSE, TRUE,
                       padding);
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       nanosecondsPerSampleLabel, FALSE, TRUE,
                       padding);

    gtk_container_add(GTK_CONTAINER(debugDataControlsExpander),
                      measurementsVerticalBox);
    return debugDataControlsExpander;
}
