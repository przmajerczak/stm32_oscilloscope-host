#include "src/DataRetriever.hpp"
#include "src/DisplayHelper.hpp"


int main(int argc, char **argv)
{
    DisplayHelper::init(argc, argv);

    DataRetriever dataRetriever; 
    dataRetriever.runContinuousDataRetrieve();

    DisplayHelper::run();
    return 0;
}
