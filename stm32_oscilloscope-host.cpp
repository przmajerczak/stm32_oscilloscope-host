#include "src/DataRetriever.hpp"
#include "src/DisplayHelper.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"

int main(int argc, char **argv)
{
    DisplayHelper::init(argc, argv);

    DataRetriever dataRetriever;
    SettingsWindow settingsWindow{};

    dataRetriever.runContinuousDataRetrieve();
    settingsWindow.runAsSeparateThread();
    DisplayHelper::run();

    return 0;
}
