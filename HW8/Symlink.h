#ifndef SYMLINK_H
#define SYMLINK_H
#include "Fing.h"

class Symlink : public Fing {
public:

    Symlink(const std::string );

    virtual ~Symlink()=default;

    std::string type() const;
};


#endif
