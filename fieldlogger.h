#ifndef FIELDLOGGER_H
#define FIELDLOGGER_H

#include "file.h"

class FieldLogger
{
public:
    FieldLogger(File *file);

    void operator<<(std::string str);

private:
    File* log;
};

#endif // FIELDLOGGER_H
