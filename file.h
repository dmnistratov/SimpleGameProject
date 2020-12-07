#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <QTextStream>

class File
{
public:
    File(const char* filename)
    {
        log = new std::ofstream(filename, std::ios::app);
        if(!log->is_open())
            throw std::runtime_error("file open failure");
    }

    ~File()
    {
        log->close();
    }

    void write (const std::string str)
    {
        *log << str << std::endl;
    }

private:
    std::ofstream *log;
    File( const File &);
    File & operator= ( const File &);

};

#endif // FILE_H
