/**
 * Project observer
 */


#ifndef _FILELOGOBSERVER_H
#define _FILELOGOBSERVER_H

#include "string"
#include "fstream"
#include "../Framework/IObserver.h"

class FileLogObserver: public IObserver{
public:
    std::string filepath;
    FILE* file;

public:
    FileLogObserver(std::string path);

    virtual ~FileLogObserver();

public:
    void update(char *log) override;
};

#endif //_FILELOGOBSERVER_H