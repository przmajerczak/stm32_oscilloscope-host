#include "MeasurementsControls.hpp"

#include "sharedData/constants.hpp"
#include <iomanip>
#include <sstream>

gboolean frequencyLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *frequencyLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(frequencyLabel);

    std::stringstream labelContent;

    if (dynamicData->thresholdTriggersWithinFrame != 0)
    {
        double microseconds_per_period{
            dynamicData->frame_duration_us /
            static_cast<double>(dynamicData->thresholdTriggersWithinFrame)};
        double frequency_Hz{1000000 / microseconds_per_period};

        labelContent << "Signal frequency: " << std::fixed << std::setprecision(2)
                     << frequency_Hz << " Hz";
    }
    else
    {
        labelContent << "Signal frequency: --- Hz";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

void MeasurementsControls::prepare(DynamicData &dynamicData)
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);

    callbackData.widget = frequencyLabel;
    callbackData.data = &dynamicData;

    g_timeout_add(FREQUENCY_LABEL_TIMEOUT_MS, frequencyLabelTimeoutAction,
                  &callbackData);
}

GtkWidget *MeasurementsControls::getFrequencyControlsContainer()
{
    GtkWidget *measurementsControlsExpander = gtk_expander_new("Measurements");
    gtk_expander_set_expanded(GTK_EXPANDER(measurementsControlsExpander), TRUE);

    constexpr int spacing{0};
    GtkWidget *measurementsVerticalBox =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox), frequencyLabel, FALSE,
                       TRUE, padding);

    gtk_container_add(GTK_CONTAINER(measurementsControlsExpander),
                      measurementsVerticalBox);
    return measurementsControlsExpander;
}
