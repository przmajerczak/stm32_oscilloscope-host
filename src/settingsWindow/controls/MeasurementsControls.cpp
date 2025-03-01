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

gboolean minVoltageLabelTimeoutAction(gpointer _callbackData) { return TRUE; }
gboolean maxVoltageLabelTimeoutAction(gpointer _callbackData) { return TRUE; }
gboolean avgVoltageLabelTimeoutAction(gpointer _callbackData) { return TRUE; }

void MeasurementsControls::prepare(DynamicData &dynamicData)
{
    prepareFrequencyLabel(dynamicData);
    prepareMinVoltageLabel(dynamicData);
    prepareMaxVoltageLabel(dynamicData);
    prepareAvgVoltageLabel(dynamicData);
}

void MeasurementsControls::prepareFrequencyLabel(DynamicData &dynamicData)
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);

    callbackDataForFrequencyLabel.widget = frequencyLabel;
    callbackDataForFrequencyLabel.data = &dynamicData;

    g_timeout_add(MEASUREMENTS_LABELS_REFRESH_RATE_MS, frequencyLabelTimeoutAction,
                  &callbackDataForFrequencyLabel);
}
void MeasurementsControls::prepareMinVoltageLabel(DynamicData &dynamicData)
{
    minVoltageLabel = gtk_label_new("Minimal voltage: --- mV");
    gtk_widget_set_hexpand(minVoltageLabel, TRUE);

    callbackDataForMinVoltageLabel.widget = minVoltageLabel;
    callbackDataForMinVoltageLabel.data = &dynamicData;

    g_timeout_add(MEASUREMENTS_LABELS_REFRESH_RATE_MS, minVoltageLabelTimeoutAction,
                  &callbackDataForMinVoltageLabel);
}
void MeasurementsControls::prepareMaxVoltageLabel(DynamicData &dynamicData)
{
    maxVoltageLabel = gtk_label_new("Maximal voltage: --- mV");
    gtk_widget_set_hexpand(maxVoltageLabel, TRUE);

    callbackDataForMaxVoltageLabel.widget = maxVoltageLabel;
    callbackDataForMaxVoltageLabel.data = &dynamicData;

    g_timeout_add(MEASUREMENTS_LABELS_REFRESH_RATE_MS, maxVoltageLabelTimeoutAction,
                  &callbackDataForMaxVoltageLabel);
}
void MeasurementsControls::prepareAvgVoltageLabel(DynamicData &dynamicData)
{
    avgVoltageLabel = gtk_label_new("Average voltage: --- mV");
    gtk_widget_set_hexpand(avgVoltageLabel, TRUE);

    callbackDataForAvgVoltageLabel.widget = avgVoltageLabel;
    callbackDataForAvgVoltageLabel.data = &dynamicData;

    g_timeout_add(MEASUREMENTS_LABELS_REFRESH_RATE_MS, avgVoltageLabelTimeoutAction,
                  &callbackDataForAvgVoltageLabel);
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
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox), minVoltageLabel, FALSE,
                       TRUE, padding);
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox), maxVoltageLabel, FALSE,
                       TRUE, padding);
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox), avgVoltageLabel, FALSE,
                       TRUE, padding);

    gtk_container_add(GTK_CONTAINER(measurementsControlsExpander),
                      measurementsVerticalBox);
    return measurementsControlsExpander;
}
