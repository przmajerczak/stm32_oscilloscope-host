#include "src/DataRetriever.hpp"
#include "src/display/DisplayHelper.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"
#include "src/sharedData/DynamicData.hpp"

int main(int argc, char **argv)
{
    DynamicData dynamicData{};

    DataRetriever dataRetriever{};
    SettingsWindow settingsWindow{};
    DisplayHelper displayHelper{dynamicData};

    dataRetriever.runContinuousDataRetrieve(dynamicData);
    settingsWindow.runAsSeparateThread(dynamicData);
    displayHelper.run();

    return 0;
}
