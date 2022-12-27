/**
 * Project Untitled
 */


#ifndef _STREAM_H
#define _STREAM_H

class Stream {
public: 
    
virtual void read() = 0;
    
virtual void write() = 0;
    
virtual void seek() = 0;
};

#endif //_STREAM_H