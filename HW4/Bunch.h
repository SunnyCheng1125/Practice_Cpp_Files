#ifndef BUNCH_H
#define BUNCH_H

#include <iostream>
#include <vector>

class Bunch {
public:
    Bunch(const std::string, const std::string magic = "/pub/media-types", const std::string= "%p %U %G %s %n",
          const bool all = false);

    Bunch(const Bunch &);

    Bunch &operator=(const Bunch &);

    ~Bunch();

    void path(std::string);

    void magic(std::string);

    void format(std::string);

    void all(bool);

    size_t size();

    bool empty();

    std::string entry(size_t);

    void get_formatType(const std::string, const std::string, const std::string);

    void read_directory(const std::string, std::vector<std::string> &, const bool);

private:
    std::vector<std::string> filePath, newFilePath;
    std::string pathIn, magicIn, formatIn, allIn;
};

#endif
