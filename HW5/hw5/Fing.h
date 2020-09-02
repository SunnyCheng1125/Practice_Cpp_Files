#ifndef FING_H
#define FING_H

#include <iostream>
#include <vector>

template<typename T, typename U>
class Dual {
    const T t;
    const U u;
public:
    Dual(const T &tt, const U &uu) : t(tt), u(uu) {}

    operator T() const { return t; }

    operator U() const { return u; }
};


class Fing {
public:
    Fing() = delete;

    Fing(const std::string);

    Fing(const Fing &);

    Fing &operator=(const Fing &);

    ~Fing();

    std::string path() const;

    size_t size() const;

    Dual<std::string, int> perms() const;

    Dual<std::string, uid_t> uid() const;

    Dual<std::string, gid_t> gid() const;

    Dual<std::string, time_t> atime() const;

    Dual<std::string, time_t> mtime() const;

    Dual<std::string, time_t> ctime() const;

private:
    std::string fingPath;
};

#endif
