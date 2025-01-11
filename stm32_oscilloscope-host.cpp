#include "src/DataRetriever.hpp"
#include "src/DisplayHelper.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"
#include "src/sharedData/DynamicData.hpp"

int main(int argc, char **argv)
{
    DynamicData dynamicData{};

    DisplayHelper::init(argc, argv);

    DataRetriever dataRetriever{};
    SettingsWindow settingsWindow{};

    dataRetriever.runContinuousDataRetrieve(dynamicData);
    settingsWindow.runAsSeparateThread(dynamicData);
    DisplayHelper::run();

    return 0;
}
