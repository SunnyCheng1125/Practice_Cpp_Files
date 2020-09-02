#ifndef BUNCH_H
#define BUNCH_H

#include "Fing.h"
#include <iostream>
#include <vector>


class Bunch {
public:
    Bunch() = delete;

    Bunch(const std::string, const bool all = false);

    Bunch(const Bunch &);

    Bunch &operator=(const Bunch &);

    ~Bunch();

    void path(std::string);

    void all(bool a = true);

    size_t size() const;

    bool empty() const;

    Fing entry(size_t) const;

    void read_directory(const std::string, std::vector<std::string> &, const bool);

private:
    std::vector<std::string> filePath;
    std::string pathIn;
    bool allIn = false;
    std::vector<Fing> fingFilePath;
};

#endif
