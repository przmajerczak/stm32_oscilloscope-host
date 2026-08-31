#include "src/oscilloscope/Oscilloscope.hpp"
#include "src/settingsWindow/SettingsWindow.hpp"
#include "src/sharedData/DynamicData.hpp"

int main(int argc, char **argv)
{
    DynamicData dynamicData{};

    SettingsWindow settingsWindow{};
    Oscilloscope oscilloscope{dynamicData};

    settingsWindow.runAsSeparateThread(dynamicData);
    oscilloscope.runDataRetrieve();
    oscilloscope.run();

    return 0;
}
