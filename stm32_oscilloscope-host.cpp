#include "src/oscilloscope/Oscilloscope.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"
#include "src/sharedData/DynamicData.hpp"

bool testModeActive(int argc, char **argv)
{
    return argc > 1 and strcmp(argv[1], "test_mode\n");
}

int main(int argc, char **argv)
{
    DynamicData dynamicData{};

    SettingsWindow settingsWindow{};
    Oscilloscope oscilloscope{dynamicData, testModeActive(argc, argv)};

    settingsWindow.runAsSeparateThread(dynamicData);
    oscilloscope.runDataRetrieve();
    oscilloscope.run();

    return 0;
}
