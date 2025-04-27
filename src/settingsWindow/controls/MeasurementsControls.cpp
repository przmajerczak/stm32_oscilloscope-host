#include "MeasurementsControls.hpp"

#include "sharedData/constants.hpp"
#include "utils.hpp"
#include <iomanip>
#include <numeric>
#include <sstream>

gboolean frequencyLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *frequencyLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(frequencyLabel);
    std::stringstream labelContent;
    labelContent << "Signal frequency: ";

    const double frequency_Hz{
        dynamicData->signalMeasurementsData.at(CHANNEL_1).frequency_Hz};
    if (frequency_Hz > 1000000.0)
    {
        labelContent << std::fixed << std::setprecision(2)
                     << frequency_Hz / 1000000.0 << " MHz";
    }
    else if (frequency_Hz > 1000.0)
    {
        labelContent << std::fixed << std::setprecision(2) << frequency_Hz / 1000.0
                     << " kHz";
    }
    else
    {
        labelContent << std::fixed << std::setprecision(2) << frequency_Hz << " Hz";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean minVoltageLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *minVoltageLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(minVoltageLabel);
    std::stringstream labelContent;

    const auto min_value{
        dynamicData->signalMeasurementsData.at(CHANNEL_1).min_value};

    if (min_value != INVALID_VALUE)
    {
        const auto min_voltage_mV = scaleYToVoltage_mV(
            *dynamicData, scaleAdcValueToY(*dynamicData, min_value));

        labelContent << "Minimal voltage: " << min_voltage_mV << " mV";
    }
    else
    {
        labelContent << "Minimal voltage: --- mV";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean maxVoltageLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *maxVoltageLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(maxVoltageLabel);
    std::stringstream labelContent;

    const auto max_voltage =
        std::max_element(dynamicData->retrieved_adc_values.at(CHANNEL_1).begin(),
                         dynamicData->retrieved_adc_values.at(CHANNEL_1).end());
    if (max_voltage != dynamicData->retrieved_adc_values.at(CHANNEL_1).end())
    {
        labelContent << "Maximal voltage: "
                     << scaleYToVoltage_mV(
                            *dynamicData,
                            scaleAdcValueToY(*dynamicData, *max_voltage))
                     << " mV";
    }
    else
    {
        labelContent << "Maximal voltage: --- mV";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

gboolean avgVoltageLabelTimeoutAction(gpointer _callbackData)
{
    CallbackData<DynamicData> *callbackData =
        (CallbackData<DynamicData> *)_callbackData;
    GtkWidget *avgVoltageLabel = callbackData->widget;
    DynamicData *dynamicData = callbackData->data;

    GtkLabel *label = GTK_LABEL(avgVoltageLabel);
    std::stringstream labelContent;

    if (dynamicData->retrieved_adc_values.at(CHANNEL_1).empty())
    {
        labelContent << "Average voltage: --- mV";
    }
    else
    {
        const auto avg_voltage{
            std::accumulate(dynamicData->retrieved_adc_values.at(CHANNEL_1).begin(),
                            dynamicData->retrieved_adc_values.at(CHANNEL_1).end(),
                            0) /
            dynamicData->retrieved_adc_values.at(CHANNEL_1).size()};

        labelContent << "Average voltage: "
                     << scaleYToVoltage_mV(
                            *dynamicData,
                            scaleAdcValueToY(*dynamicData, avg_voltage))
                     << " mV";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    return TRUE;
}

void MeasurementsControls::prepare(DynamicData &dynamicData)
{
    prepareFrequencyLabel(dynamicData);
    prepareMinVoltageLabel(dynamicData);
    prepareMaxVoltageLabel(dynamicData);
    prepareAvgVoltageLabel(dynamicData);

    verticalMeasurements.prepare(dynamicData);
    horizontalMeasurements.prepare(dynamicData);
}

void MeasurementsControls::prepareFrequencyLabel(DynamicData &dynamicData)
{
    frequencyLabel = gtk_label_new("Signal frequency: --- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);

    callbackDataForFrequencyLabel.widget = frequencyLabel;
    callbackDataForFrequencyLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, frequencyLabelTimeoutAction,
                  &callbackDataForFrequencyLabel);
}
void MeasurementsControls::prepareMinVoltageLabel(DynamicData &dynamicData)
{
    minVoltageLabel = gtk_label_new("Minimal voltage: --- mV");
    gtk_widget_set_hexpand(minVoltageLabel, TRUE);

    callbackDataForMinVoltageLabel.widget = minVoltageLabel;
    callbackDataForMinVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, minVoltageLabelTimeoutAction,
                  &callbackDataForMinVoltageLabel);
}
void MeasurementsControls::prepareMaxVoltageLabel(DynamicData &dynamicData)
{
    maxVoltageLabel = gtk_label_new("Maximal voltage: --- mV");
    gtk_widget_set_hexpand(maxVoltageLabel, TRUE);

    callbackDataForMaxVoltageLabel.widget = maxVoltageLabel;
    callbackDataForMaxVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, maxVoltageLabelTimeoutAction,
                  &callbackDataForMaxVoltageLabel);
}
void MeasurementsControls::prepareAvgVoltageLabel(DynamicData &dynamicData)
{
    avgVoltageLabel = gtk_label_new("Average voltage: --- mV");
    gtk_widget_set_hexpand(avgVoltageLabel, TRUE);

    callbackDataForAvgVoltageLabel.widget = avgVoltageLabel;
    callbackDataForAvgVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, avgVoltageLabelTimeoutAction,
                  &callbackDataForAvgVoltageLabel);
}

GtkWidget *MeasurementsControls::getMeasurementsControlsContainer()
{
    GtkWidget *measurementsControlsExpander = gtk_expander_new("Measurements");
    gtk_expander_set_expanded(GTK_EXPANDER(measurementsControlsExpander), TRUE);

    constexpr int spacing{0};
    GtkWidget *measurementsVerticalBox =
        gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       verticalMeasurements.getVerticalMeasurementsContainer(),
                       FALSE, TRUE, padding);
    gtk_box_pack_start(
        GTK_BOX(measurementsVerticalBox),
        horizontalMeasurements.getHorizontalMeasurementsContainer(), FALSE, TRUE,
        padding);
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
