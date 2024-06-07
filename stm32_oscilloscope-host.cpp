#include "src/DataRetriever.hpp"
#include "src/DisplayHelper.hpp"


int main(int argc, char **argv)
{
    DisplayHelper::init(argc, argv);

    DataRetriever dataRetriever; 
    dataRetriever.runContinuousDataRetrieve(DATA_RETRIEVAL_TIMER_VALUE_MS);

    DisplayHelper::run();
    return 0;
}
