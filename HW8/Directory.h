#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "Fing.h"

class Directory :public Fing{
public:

    Directory(const std::string);

    virtual ~Directory()=default;

    std::string type() const;
};




#endif
