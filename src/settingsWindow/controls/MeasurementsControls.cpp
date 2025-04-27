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

    const double frequency_Hz{
        dynamicData->signalMeasurementsData.at(CHANNEL_1).frequency_Hz};

    if (frequency_Hz >= 0.0)
    {
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
    }
    else
    {
        labelContent << "--- Hz";
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

        labelContent << min_voltage_mV << " mV";
    }
    else
    {
        labelContent << "--- mV";
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

    const auto max_value{
        dynamicData->signalMeasurementsData.at(CHANNEL_1).max_value};

    if (max_value != INVALID_VALUE)
    {
        const auto max_voltage_mV = scaleYToVoltage_mV(
            *dynamicData, scaleAdcValueToY(*dynamicData, max_value));

        labelContent << max_voltage_mV << " mV";
    }
    else
    {
        labelContent << "--- mV";
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

    if (dynamicData->signalMeasurementsData.at(CHANNEL_1).average_value ==
        INVALID_VALUE)
    {
        labelContent << "--- mV";
    }
    else
    {
        labelContent << scaleYToVoltage_mV(
                            *dynamicData,
                            scaleAdcValueToY(
                                *dynamicData,
                                dynamicData->signalMeasurementsData.at(CHANNEL_1)
                                    .average_value))
                     << " mV";
    }

    gtk_label_set_text(label, labelContent.str().c_str());

    dynamicData->signalMeasurementsData.at(CHANNEL_1).reset();

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
    frequencyLabel = gtk_label_new("--- Hz");
    gtk_widget_set_hexpand(frequencyLabel, TRUE);

    callbackDataForFrequencyLabel.widget = frequencyLabel;
    callbackDataForFrequencyLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, frequencyLabelTimeoutAction,
                  &callbackDataForFrequencyLabel);
}
void MeasurementsControls::prepareMinVoltageLabel(DynamicData &dynamicData)
{
    minVoltageLabel = gtk_label_new("--- mV");
    gtk_widget_set_hexpand(minVoltageLabel, TRUE);

    callbackDataForMinVoltageLabel.widget = minVoltageLabel;
    callbackDataForMinVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, minVoltageLabelTimeoutAction,
                  &callbackDataForMinVoltageLabel);
}
void MeasurementsControls::prepareMaxVoltageLabel(DynamicData &dynamicData)
{
    maxVoltageLabel = gtk_label_new("-- mV");
    gtk_widget_set_hexpand(maxVoltageLabel, TRUE);

    callbackDataForMaxVoltageLabel.widget = maxVoltageLabel;
    callbackDataForMaxVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, maxVoltageLabelTimeoutAction,
                  &callbackDataForMaxVoltageLabel);
}
void MeasurementsControls::prepareAvgVoltageLabel(DynamicData &dynamicData)
{
    avgVoltageLabel = gtk_label_new("--- mV");
    gtk_widget_set_hexpand(avgVoltageLabel, TRUE);

    callbackDataForAvgVoltageLabel.widget = avgVoltageLabel;
    callbackDataForAvgVoltageLabel.data = &dynamicData;

    g_timeout_add(LABELS_REFRESH_RATE_MS, avgVoltageLabelTimeoutAction,
                  &callbackDataForAvgVoltageLabel);
}

GtkWidget *MeasurementsControls::getMeasurementsGridContainer()
{
    auto title_label_f = gtk_label_new("f");
    auto title_label_max = gtk_label_new("max");
    auto title_label_avg = gtk_label_new("avg");
    auto title_label_min = gtk_label_new("min");
    auto title_label_amplitude = gtk_label_new("amplitude");

    auto title_label_CH1 = gtk_label_new("CH1");
    auto title_label_CH2 = gtk_label_new("CH2");

    gtk_widget_set_hexpand(title_label_f, TRUE);
    gtk_widget_set_hexpand(title_label_max, TRUE);
    gtk_widget_set_hexpand(title_label_avg, TRUE);
    gtk_widget_set_hexpand(title_label_min, TRUE);
    gtk_widget_set_hexpand(title_label_amplitude, TRUE);
    gtk_widget_set_hexpand(title_label_CH1, TRUE);
    gtk_widget_set_hexpand(title_label_CH2, TRUE);

    GtkWidget *measurementsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_f, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_min, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_avg, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_max, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_amplitude, 0, 5, 1,
                    1);

    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_CH1, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), frequencyLabel, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), maxVoltageLabel, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), avgVoltageLabel, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), minVoltageLabel, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_CH2, 2, 0, 1, 1);

    return measurementsGrid;
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

    gtk_box_pack_start(GTK_BOX(measurementsVerticalBox),
                       getMeasurementsGridContainer(), FALSE, TRUE, padding);

    gtk_container_add(GTK_CONTAINER(measurementsControlsExpander),
                      measurementsVerticalBox);
    return measurementsControlsExpander;
}
