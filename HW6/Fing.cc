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

Fing::Fing(const std::string fingP) {
    fingPath = fingP;
    const char *fingp = fingPath.c_str();
    int result = lstat(fingp, &statbuf);
    pwu = getpwuid(statbuf.st_uid);
    pwg = getgrgid(statbuf.st_gid);

    if (result != 0) {
        throw string(" lstat file path: " + fingPath + " failed");
    }
}

Fing::Fing(const Fing &FingC) {
    fingPath = FingC.fingPath;
    statbuf = FingC.statbuf;
    pwu = FingC.pwu;
    pwg = FingC.pwg;
}

Fing &Fing::operator=(const Fing &FingA) {
    fingPath = FingA.fingPath;
    statbuf = FingA.statbuf;
    pwu = FingA.pwu;
    pwg = FingA.pwg;
    return *this;
}

Fing::~Fing() {

}

string Fing::path() const {
    return fingPath;
}

size_t Fing::size() const {
    return statbuf.st_size;
}

Dual<string, int> Fing::perms() const {
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
    //user id
    return {string(pwu->pw_name), statbuf.st_uid};
}

Dual<string, gid_t> Fing::gid() const {
    //group id
    return {string(pwg->gr_name), statbuf.st_gid};
}

Dual<string, time_t> Fing::atime() const {
    //access time
    time_t a = statbuf.st_atime;
    auto atimevals = localtime(&a);
    char abuf[32];
    strftime(abuf, sizeof(abuf), "%FT%T", atimevals);
    //abuf is atime
    return {abuf, statbuf.st_atime};
}

Dual<string, time_t> Fing::mtime() const {
    //modified time
    time_t m = statbuf.st_mtime;
    auto mtimevals = localtime(&m);
    char mbuf[32];
    strftime(mbuf, sizeof(mbuf), "%FT%T", mtimevals);
    //mbuf is atime
    return {mbuf, statbuf.st_mtime};
}

Dual<string, time_t> Fing::ctime() const {
    //changed time
    time_t c = statbuf.st_ctime;
    auto ctimevals = localtime(&c);
    char cbuf[32];
    strftime(cbuf, sizeof(cbuf), "%FT%T", ctimevals);
    //cbuf is atime
    return {cbuf, statbuf.st_ctime};
}

dev_t Fing::getDev() const {
    return statbuf.st_dev;
}

__ino64_t Fing::getIno() const {
    return statbuf.st_ino;
}

bool Fing::operator==(const Fing &input) const {
    return getDev() == input.getDev() && getIno() == input.getIno();
}