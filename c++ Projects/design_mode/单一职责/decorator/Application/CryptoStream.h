/**
 * Project Untitled
 */


#ifndef _CRYPTOSTREAM_H
#define _CRYPTOSTREAM_H

#include "../Framework/Decorator.h"


class CryptoStream: public Decorator {
public:
    CryptoStream(Stream *stream);

    void read() override;

    void write() override;

    void seek() override;
};

#endif //_CRYPTOSTREAM_H