/**
 * Project factory_method
 */


#include "TxtSplitterFactory.h"
#include "TxtSplitter.h"
#include "PrintFunction.h"

/**
 * TxtSplitterFactory implementation
 */

Splitter *TxtSplitterFactory::createInstance()
{
    PrintFunctionPath();
    return new TxtSplitter;
}