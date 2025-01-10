#include "src/DataRetriever.hpp"
#include "src/DisplayHelper.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"

int main(int argc, char **argv)
{
    DisplayHelper::init(argc, argv);

    DataRetriever dataRetriever;
    dataRetriever.runContinuousDataRetrieve();

    SettingsWindow::run();
    DisplayHelper::run();

    return 0;
}
