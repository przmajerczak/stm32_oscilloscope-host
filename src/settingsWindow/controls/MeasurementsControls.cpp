#include "MeasurementsControls.hpp"

#include "sharedData/constants.hpp"
#include "utils.hpp"
#include <iomanip>
#include <numeric>
#include <sstream>

void fillFrequencyLabel(const LabelsPointers &labels_pointers,
                        DynamicData &dynamicData, const ChannelId channelId)
{
    std::stringstream labelContent;

    const double frequency_Hz{
        dynamicData.signalMeasurementsData.at(channelId).frequency_Hz};

    if (frequency_Hz >= 0.0)
    {
        if (frequency_Hz > 1000000.0)
        {
            labelContent << std::fixed << std::setprecision(2)
                         << frequency_Hz / 1000000.0 << " MHz";
        }
        else if (frequency_Hz > 1000.0)
        {
            labelContent << std::fixed << std::setprecision(2)
                         << frequency_Hz / 1000.0 << " kHz";
        }
        else
        {
            labelContent << std::fixed << std::setprecision(2) << frequency_Hz
                         << " Hz";
        }
    }
    else
    {
        labelContent << "--- Hz";
    }

    GtkLabel *frequencyLabel =
        GTK_LABEL(labels_pointers.at(channelId).frequencyLabel);

    gtk_label_set_text(frequencyLabel, labelContent.str().c_str());
}

void fillMaxVoltageLabel(const LabelsPointers &labels_pointers,
                         DynamicData &dynamicData, const ChannelId channelId)
{
    GtkLabel *maxVoltageLabel =
        GTK_LABEL(labels_pointers.at(channelId).maxVoltageLabel);
    std::stringstream maxVoltageLabelContent;

    const auto max_value{
        dynamicData.signalMeasurementsData.at(channelId).max_value};

    if (max_value != INVALID_VALUE)
    {
        const auto max_voltage_mV = scaleYToVoltage_mV(
            dynamicData, scaleAdcValueToY(dynamicData, max_value));

        maxVoltageLabelContent << max_voltage_mV << " mV";
    }
    else
    {
        maxVoltageLabelContent << "--- mV";
    }

    gtk_label_set_text(maxVoltageLabel, maxVoltageLabelContent.str().c_str());
}

void fillAvgVoltageLabel(const LabelsPointers &labels_pointers,
                         DynamicData &dynamicData, const ChannelId channelId)
{
    GtkLabel *avgVoltageLabel =
        GTK_LABEL(labels_pointers.at(channelId).avgVoltageLabel);
    std::stringstream avgVoltageLabelContent;

    if (dynamicData.signalMeasurementsData.at(channelId).average_value ==
        INVALID_VALUE)
    {
        avgVoltageLabelContent << "--- mV";
    }
    else
    {
        avgVoltageLabelContent
            << scaleYToVoltage_mV(
                   dynamicData,
                   scaleAdcValueToY(dynamicData,
                                    dynamicData.signalMeasurementsData.at(channelId)
                                        .average_value))
            << " mV";
    }

    gtk_label_set_text(avgVoltageLabel, avgVoltageLabelContent.str().c_str());
}

void fillMinVoltageLabel(const LabelsPointers &labels_pointers,
                         DynamicData &dynamicData, const ChannelId channelId)
{
    GtkLabel *minVoltageLabel =
        GTK_LABEL(labels_pointers.at(channelId).minVoltageLabel);
    std::stringstream minVoltageLabelContent;

    const auto min_value{
        dynamicData.signalMeasurementsData.at(channelId).min_value};

    if (min_value != INVALID_VALUE)
    {
        const auto min_voltage_mV = scaleYToVoltage_mV(
            dynamicData, scaleAdcValueToY(dynamicData, min_value));

        minVoltageLabelContent << min_voltage_mV << " mV";
    }
    else
    {
        minVoltageLabelContent << "--- mV";
    }

    gtk_label_set_text(minVoltageLabel, minVoltageLabelContent.str().c_str());
}

gboolean labelsTimeoutAction(gpointer _callbackData)
{
    LabelsAndDynamicData *labels_and_dynamic_data =
        (LabelsAndDynamicData *)_callbackData;
    DynamicData *dynamicData = labels_and_dynamic_data->dynamicData;

    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        fillFrequencyLabel(*(labels_and_dynamic_data->labels_pointers),
                           *dynamicData, chId);
        fillMaxVoltageLabel(*(labels_and_dynamic_data->labels_pointers),
                            *dynamicData, chId);
        fillAvgVoltageLabel(*(labels_and_dynamic_data->labels_pointers),
                            *dynamicData, chId);
        fillMinVoltageLabel(*(labels_and_dynamic_data->labels_pointers),
                            *dynamicData, chId);

        dynamicData->signalMeasurementsData.at(chId).reset();
    }

    return TRUE;
}

void MeasurementsControls::prepare(DynamicData &dynamicData)
{
    labels_and_dynamic_data.dynamicData = &dynamicData;
    labels_and_dynamic_data.labels_pointers = &labels_pointers;

    prepareLabels(dynamicData);

    verticalMeasurements.prepare(dynamicData);
    horizontalMeasurements.prepare(dynamicData);

    g_timeout_add(LABELS_REFRESH_RATE_MS, labelsTimeoutAction,
                  &labels_and_dynamic_data);
}

void MeasurementsControls::prepareLabels(DynamicData &dynamicData)
{
    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        auto &labels_for_single_channel{labels_pointers.at(chId)};

        labels_for_single_channel.frequencyLabel = gtk_label_new("--- Hz");
        gtk_widget_set_hexpand(labels_for_single_channel.frequencyLabel, TRUE);

        labels_for_single_channel.minVoltageLabel = gtk_label_new("--- mV");
        gtk_widget_set_hexpand(labels_for_single_channel.minVoltageLabel, TRUE);

        labels_for_single_channel.maxVoltageLabel = gtk_label_new("-- mV");
        gtk_widget_set_hexpand(labels_for_single_channel.maxVoltageLabel, TRUE);

        labels_for_single_channel.avgVoltageLabel = gtk_label_new("--- mV");
        gtk_widget_set_hexpand(labels_for_single_channel.avgVoltageLabel, TRUE);
    }
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
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_1).frequencyLabel, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_1).maxVoltageLabel, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_1).avgVoltageLabel, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_1).minVoltageLabel, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_CH2, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_2).frequencyLabel, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_2).maxVoltageLabel, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_2).avgVoltageLabel, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid),
                    labels_pointers.at(CHANNEL_2).minVoltageLabel, 2, 4, 1, 1);

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
