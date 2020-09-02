#ifndef REGULAR_H
#define REGULAR_H
#include "Fing.h"

class Regular : public Fing {
public:

    Regular(const std::string );

    ~Regular();

    std::string type() const;
};


#endif