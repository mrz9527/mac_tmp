/**
 * Project observer
 */


#include "FileLogObserver.h"

/**
 * FileLogObserver implementation
 */
void FileLogObserver::update(char *log)
{
    if(file) {
        fwrite(log, strlen(log), 1, file);
    }
}

FileLogObserver::FileLogObserver(std::string filepath) : filepath(filepath), file(nullptr)
{
    file = fopen(filepath.c_str(), "w+");
}

FileLogObserver::~FileLogObserver()
{
    if(file) {
        fclose(file);
    }
}
