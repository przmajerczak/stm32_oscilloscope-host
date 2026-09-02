#include "MeasurementsControls.hpp"

#include <iomanip>
#include <numeric>
#include <sstream>

#include "sharedData/constants.hpp"
#include "utils.hpp"

void fillFrequencyLabel(const GtkWidget *label_pointer,
                        const double frequency_Hz, const bool channel_active)
{
    std::stringstream labelContent;
    labelContent << std::fixed << std::setprecision(2);

    if (channel_active and frequency_Hz >= 0.0)
    {
        if (frequency_Hz > 1000000.0)
        {
            labelContent << frequency_Hz / 1000000.0 << " MHz";
        }
        else if (frequency_Hz > 1000.0)
        {
            labelContent << frequency_Hz / 1000.0 << " kHz";
        }
        else
        {
            labelContent << frequency_Hz << " Hz";
        }
    }
    else
    {
        labelContent << "--- Hz";
    }

    gtk_label_set_text(GTK_LABEL(label_pointer), labelContent.str().c_str());
}

void fillVoltageLabel(const GtkWidget *label_pointer, const float value,
                      const bool channel_active)
{
    std::stringstream labelContent;

    if (channel_active)
    {
        labelContent << static_cast<int>(value) << " mV";
    }
    else
    {
        labelContent << "--- mV";
    }

    gtk_label_set_text(GTK_LABEL(label_pointer), labelContent.str().c_str());
}

gboolean labelsTimeoutAction(gpointer _callbackData)
{
    LabelsAndDynamicData *labels_and_dynamic_data =
        (LabelsAndDynamicData *)_callbackData;
    DynamicData *dynamicData = labels_and_dynamic_data->dynamicData;
    LabelsPointers *labels_pointers = labels_and_dynamic_data->labels_pointers;

    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        const bool channel_active{dynamicData->active_channels.at(chId)};
        const LabelsForSingleChannel &labels{labels_pointers->at(chId)};
        const SignalMeasurementsData &measurements{
            dynamicData->signalMeasurementsData.at(chId)};

        fillFrequencyLabel(labels.frequencyLabel, measurements.frequency_Hz,
                           channel_active);
        fillVoltageLabel(labels.maxVoltageLabel, measurements.max_value_mV,
                         channel_active);
        fillVoltageLabel(labels.avgVoltageLabel, measurements.average_value_mV,
                         channel_active);
        fillVoltageLabel(labels.minVoltageLabel, measurements.min_value_mV,
                         channel_active);
        fillVoltageLabel(labels.amplitudeLabel, measurements.amplitude_mV,
                         channel_active);
    }

    return TRUE;
}

void MeasurementsControls::prepare(DynamicData &dynamicData)
{
    labels_and_dynamic_data.dynamicData = &dynamicData;
    labels_and_dynamic_data.labels_pointers = &labels_pointers;

    prepareLabels(dynamicData);

    g_timeout_add(LABELS_REFRESH_RATE_MS, labelsTimeoutAction,
                  &labels_and_dynamic_data);
}

void MeasurementsControls::prepareLabels(DynamicData &dynamicData)
{
    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        auto &channel_labels{labels_pointers.at(chId)};

        std::stringstream title;
        title << "CH" << (chId + 1);

        channel_labels.titleLabel = gtk_label_new(title.str().c_str());
        gtk_widget_set_hexpand(channel_labels.titleLabel, TRUE);

        channel_labels.frequencyLabel = gtk_label_new("--- Hz");
        gtk_widget_set_hexpand(channel_labels.frequencyLabel, TRUE);

        channel_labels.maxVoltageLabel = gtk_label_new("-- mV");
        gtk_widget_set_hexpand(channel_labels.maxVoltageLabel, TRUE);

        channel_labels.avgVoltageLabel = gtk_label_new("--- mV");
        gtk_widget_set_hexpand(channel_labels.avgVoltageLabel, TRUE);

        channel_labels.minVoltageLabel = gtk_label_new("--- mV");
        gtk_widget_set_hexpand(channel_labels.minVoltageLabel, TRUE);

        channel_labels.amplitudeLabel = gtk_label_new("--- mV");
        gtk_widget_set_hexpand(channel_labels.amplitudeLabel, TRUE);
    }
}

GtkWidget *MeasurementsControls::getMeasurementsGridContainer()
{
    auto title_label_f = gtk_label_new("f");
    auto title_label_max = gtk_label_new("max");
    auto title_label_avg = gtk_label_new("avg");
    auto title_label_min = gtk_label_new("min");
    auto title_label_amplitude = gtk_label_new("amplitude");

    gtk_widget_set_hexpand(title_label_f, TRUE);
    gtk_widget_set_hexpand(title_label_max, TRUE);
    gtk_widget_set_hexpand(title_label_avg, TRUE);
    gtk_widget_set_hexpand(title_label_min, TRUE);
    gtk_widget_set_hexpand(title_label_amplitude, TRUE);

    GtkWidget *measurementsGrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_f, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_max, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_avg, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_min, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(measurementsGrid), title_label_amplitude, 0, 5, 1,
                    1);

    for (ChannelId chId = CHANNEL_1; chId < NUMBER_OF_CHANNELS; ++chId)
    {
        const auto &channel_labels{labels_pointers.at(chId)};

        const gint column{1 + static_cast<gint>(chId)};
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.titleLabel,
                        column, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.frequencyLabel,
                        column, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.maxVoltageLabel,
                        column, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.avgVoltageLabel,
                        column, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.minVoltageLabel,
                        column, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(measurementsGrid), channel_labels.amplitudeLabel,
                        column, 5, 1, 1);
    }

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
                       getMeasurementsGridContainer(), FALSE, TRUE, padding);

    gtk_container_add(GTK_CONTAINER(measurementsControlsExpander),
                      measurementsVerticalBox);

    return measurementsControlsExpander;
}
