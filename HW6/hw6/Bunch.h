#ifndef BUNCH_H
#define BUNCH_H

#include "Fing.h"
#include <iostream>
#include <vector>


class Bunch {
public:
    Bunch() = delete;

    Bunch(const std::string);

    Bunch(const Bunch &);

    Bunch &operator=(const Bunch &);

    ~Bunch();

    size_t size() const;

    bool empty() const;

    const Fing *entry(size_t) const;


    bool operator==(const Bunch &) const;

    bool operator!=(const Bunch &) const;

    Bunch operator+(const Bunch &) const;

    Bunch operator-(const Bunch &) const;

    Bunch &operator+=(const Bunch &);

    Bunch &operator-=(const Bunch &);

    operator bool() const;

    void read_directory(const std::string, std::vector<std::string> &, const bool);

private:
    std::vector<std::string> filePath;//input path
    std::string pathIn;
    bool allIn = false;
    std::vector<Fing> fingFilePath;//after Fing class, new path
};

#endif
