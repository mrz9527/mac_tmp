/**
 * Project Untitled
 */


#include "BufferedStream.h"
#include "PrintFunction.h"
#include "iostream"
/**
 * BufferedStream implementation
 */
void BufferedStream::read()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对read缓冲"<<std::endl;
    stream->read();
}

void BufferedStream::write()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对write缓冲"<<std::endl;
    stream->write();
}

void BufferedStream::seek()
{
    //PrintFunctionPath();
    if (!stream) return;

    std::cout<<"对seek缓冲"<<std::endl;
    stream->seek();
}

BufferedStream::BufferedStream(Stream *stream) : Decorator(stream)
{}
