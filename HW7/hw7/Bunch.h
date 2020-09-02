#ifndef BUNCH_H
#define BUNCH_H

#include "Fing.h"
#include "Regular.h"
#include "Directory.h"
#include "Symlink.h"
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

    class iterator {
    public:
        iterator(std::vector<Fing *> fingP,int ptr = 0) : p(ptr), fingFilePath(fingP){}

        iterator &operator++() {    // Preincrement
            p++;
            return *this;
        }

        iterator operator++(int) {    // Postincrement
            const auto save = *this;
            ++*this;            // Let preincrement do the work
            return save;
        }

        iterator &operator--() {    // Preincrement
            p--;
            return *this;
        }

        iterator operator--(int) {    // Postincrement
            const auto save = *this;
            --*this;            // Let preincrement do the work
            return save;
        }

        // *iterator: Return a reference to the datum
        const Fing* operator*() const {
            return fingFilePath[p];
        }

        bool operator==(const iterator &rhs) const {
            return p == rhs.p;
        }

        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);            // Let == do the work.
        }

    private:
        int p;
        std::vector<Fing *> fingFilePath;
    };


    iterator begin() const;

    iterator end() const;

private:
    std::vector<std::string> filePath;//input path
    std::string pathIn;
    bool allIn = false;
    std::vector<Fing *> fingFilePath;//after Fing class, new path //const Fing *
};

#endif
