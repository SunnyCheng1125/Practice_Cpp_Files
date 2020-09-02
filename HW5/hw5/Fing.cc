#include "Bunch.h"
#include "Fing.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

using namespace std;
struct stat statbuf;
struct passwd *pwu;
struct group *pwg;

Fing::Fing(const std::string fingP) {
    fingPath = fingP;
}

Fing::Fing(const Fing &FingC) {
    fingPath = FingC.fingPath;
}

Fing &Fing::operator=(const Fing &FingA) {
    fingPath = FingA.fingPath;
    return *this;
}

Fing::~Fing() {

}

string Fing::path() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");

    }
    return fingPath;
}

size_t Fing::size() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");

    }
    return statbuf.st_size;
}

Dual<string, int> Fing::perms() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    string permission;
    //permission
    switch (statbuf.st_mode & S_IFMT) {
        case S_IFREG:
            permission += "-";
            break;
        case S_IFDIR:
            permission += "d";
            break;
        case S_IFLNK:
            permission += "l";
            break;
        default:
            //throw string(" : Undifined " + argvS + " file!");
            break;
    }
    if (statbuf.st_mode & S_IRUSR) {
        permission += "r";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IWUSR) {
        permission += "w";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IXUSR) {
        permission += "x";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IRGRP) {
        permission += "r";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IWGRP) {
        permission += "w";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IXGRP) {
        permission += "x";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IROTH) {
        permission += "r";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IWOTH) {
        permission += "w";
    } else {
        permission += "-";
    }
    if (statbuf.st_mode & S_IXOTH) {
        permission += "x";
    } else {
        permission += "-";
    }
    return {permission, static_cast<int> (statbuf.st_mode)};
    //end of permission section
}

Dual<string, uid_t> Fing::uid() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    //user id
    pwu = getpwuid(statbuf.st_uid);
    return {std::to_string(statbuf.st_uid), statbuf.st_uid};
}

Dual<string, gid_t> Fing::gid() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    //group id
    pwg = getgrgid(statbuf.st_gid);
    return {std::to_string(statbuf.st_gid), statbuf.st_gid};
}

Dual<string, time_t> Fing::atime() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    //access time
    time_t a = statbuf.st_atime;
    auto atimevals = localtime(&a);
    char abuf[32];
    strftime(abuf, sizeof(abuf), "%FT%T", atimevals);
    //abuf is atime
    return {abuf, statbuf.st_atime};
}

Dual<string, time_t> Fing::mtime() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    //modified time
    time_t m = statbuf.st_mtime;
    auto mtimevals = localtime(&m);
    char mbuf[32];
    strftime(mbuf, sizeof(mbuf), "%FT%T", mtimevals);
    //mbuf is atime
    return {mbuf, statbuf.st_mtime};
}

Dual<string, time_t> Fing::ctime() const {
    const char *file;
    file = fingPath.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
    //changed time
    time_t c = statbuf.st_ctime;
    auto ctimevals = localtime(&c);
    char cbuf[32];
    strftime(cbuf, sizeof(cbuf), "%FT%T", ctimevals);
    //cbuf is atime
    return {cbuf, statbuf.st_ctime};
}
