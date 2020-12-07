#include "fieldlogger.h"


FieldLogger::FieldLogger(File *file)
{
    log = file;
}

void FieldLogger::operator<<(std::string str)
{
    std::string fields = "[FIELD] ";
    fields.append(str);
    log->write(fields);
}
