/**
 * Project Untitled
 */


#ifndef _MEMORYSTREAM_H
#define _MEMORYSTREAM_H

#include "Stream.h"


class MemoryStream: public Stream {
public:
    void read() override;

    void write() override;

    void seek() override;
};

#endif //_MEMORYSTREAM_H