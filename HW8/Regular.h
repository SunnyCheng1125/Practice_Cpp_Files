#ifndef REGULAR_H
#define REGULAR_H
#include "Fing.h"

class Regular : public Fing {
public:

    Regular(const std::string );

    virtual ~Regular()=default;

    std::string type() const;
};


#endif
