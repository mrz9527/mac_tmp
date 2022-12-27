/**
 * Project Untitled
 */


#ifndef _BUFFEREDSTREAM_H
#define _BUFFEREDSTREAM_H

#include "../Framework/Decorator.h"


class BufferedStream: public Decorator {
public:
    BufferedStream(Stream *stream);

    void read() override;

    void write() override;

    void seek() override;
};

#endif //_BUFFEREDSTREAM_H