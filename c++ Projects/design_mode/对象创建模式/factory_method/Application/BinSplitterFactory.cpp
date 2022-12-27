/**
 * Project factory_method
 */


#include "BinSplitterFactory.h"
#include "BinSplitter.h"
#include "PrintFunction.h"

/**
 * BinSplitterFactory implementation
 */
Splitter *BinSplitterFactory::createInstance()
{
    PrintFunctionPath();
    return new BinSplitter;
}
