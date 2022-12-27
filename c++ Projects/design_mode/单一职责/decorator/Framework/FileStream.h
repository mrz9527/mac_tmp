/**
 * Project Untitled
 */


#ifndef _FILESTREAM_H
#define _FILESTREAM_H

#include "Stream.h"


class FileStream: public Stream {
public:
    void read() override;

    void write() override;

    void seek() override;
};

#endif //_FILESTREAM_H