/**
 * Project Untitled
 */


#ifndef _NETWORKSTREAM_H
#define _NETWORKSTREAM_H

#include "Stream.h"


class NetworkStream: public Stream {
public:
    void read() override;

    void write() override;

    void seek() override;
};

#endif //_NETWORKSTREAM_H