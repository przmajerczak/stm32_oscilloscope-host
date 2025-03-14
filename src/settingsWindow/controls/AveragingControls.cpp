#include "AveragingControls.hpp"

void averagingWindowSizeSpinButtonOnChangeAction(GtkSpinButton *spin_button,
                                                 gpointer _dynamicData)
{
    DynamicData *dynamicData = (DynamicData *)_dynamicData;

    dynamicData->averaging_window_size = gtk_spin_button_get_value(spin_button);
}

void AveragingControls::prepare(DynamicData &dynamicData)
{
    GtkAdjustment *adjustment =
        gtk_adjustment_new(1, 1, SAMPLES_PER_TRANSMISSION, 1, 0.0, 0.0);

    averaging_window_size_spin_button = gtk_spin_button_new(adjustment, 1.0, 0);
    gtk_widget_set_hexpand(averaging_window_size_spin_button, TRUE);

    g_signal_connect(averaging_window_size_spin_button, "value-changed",
                     G_CALLBACK(averagingWindowSizeSpinButtonOnChangeAction),
                     &dynamicData);
};

GtkWidget *AveragingControls::getAveragingControlsContainer()
{
    GtkWidget *averagingExpander = gtk_expander_new("Averaging controls");
    gtk_expander_set_expanded(GTK_EXPANDER(averagingExpander), TRUE);

    constexpr int spacing{0};
    GtkWidget *averagingHorizontalBox =
        gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    constexpr int padding{0};
    gtk_box_pack_start(GTK_BOX(averagingHorizontalBox),
                       averaging_window_size_spin_button, FALSE, TRUE, padding);

    gtk_container_add(GTK_CONTAINER(averagingExpander), averagingHorizontalBox);

    return averagingExpander;
};