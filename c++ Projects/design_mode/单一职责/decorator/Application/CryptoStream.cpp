/**
 * Project Untitled
 */


#include "CryptoStream.h"
#include "iostream"
#include "PrintFunction.h"
/**
 * CryptoStream implementation
 */
void CryptoStream::read()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对read加密"<<std::endl;
    stream->read();
}

void CryptoStream::write()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对write加密"<<std::endl;
    stream->write();
}

void CryptoStream::seek()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对seek加密"<<std::endl;
    stream->seek();
}

CryptoStream::CryptoStream(Stream *stream) : Decorator(stream)
{}
