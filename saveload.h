#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "gamefield.h"

class SaveLoad
{
public:
    SaveLoad(const std::string filename)
    {
        file = new std::fstream(filename, std::ios::in | std::ios::out);
        if(!file->is_open())
            throw std::runtime_error("File doesn't exist!");
    }

    ~SaveLoad()
    {
        file->close();
    }

    void save(std::string tempstr)
    {
        *file << tempstr << std::endl;;
    }

    std::string loadline()
    {
        if (file)
        {
            std::string tempstr;
            char temp[1000];
            file->getline(temp, 1000);
            tempstr.append(temp);
            return tempstr;
        }
        return NULL;
    }

private:
    std::fstream *file;
    SaveLoad( const SaveLoad &);
    SaveLoad & operator= ( const SaveLoad &);

};

#endif // SAVELOAD_H
