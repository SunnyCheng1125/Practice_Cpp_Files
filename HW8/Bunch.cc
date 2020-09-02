#include "Bunch.h"
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

Bunch::Bunch(const string pAth) {
    pathIn = pAth;
    allIn = false;
    filePath = filePath;

    if (pathIn[0] == '.')
        throw string("It's a hidden file!");

    filePath.push_back(pAth);//current path
    read_directory(pAth, filePath, allIn);

    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing::create(filePath[val]));
    }
}

Bunch::Bunch(const Bunch &bunchC) {
    pathIn = bunchC.pathIn;
    allIn = bunchC.allIn;
    filePath = bunchC.filePath;
    //fingFilePath = bunchC.fingFilePath;

    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing::create(bunchC.filePath[val]));
    }
}

Bunch &Bunch::operator=(const Bunch &bunchA) {
    if (this == &bunchA) {
        return *this;
    }
    pathIn = bunchA.pathIn;
    allIn = bunchA.allIn;
    filePath = bunchA.filePath;
    //fingFilePath = bunchA.fingFilePath;
    for (auto i :fingFilePath) {
        delete i;
    }
    fingFilePath.clear();
    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing::create(bunchA.filePath[val]));
    }
    return *this;
}

Bunch::~Bunch() {
    for (auto i :fingFilePath) {
        delete i;
    }
    fingFilePath.clear();
}

size_t Bunch::size() const {
    return fingFilePath.size();
}

//Return true iff this Bunch has no entries.
bool Bunch::empty() const {
    if (fingFilePath.size() == 0)
        return true;
    else
        return false;
}

const Fing *Bunch::entry(size_t t) const {
    if (t >= fingFilePath.size()) {
        string s = to_string(t);
        throw string(
                "Wrong entry size " + s + " . The maximum size should be " + to_string(fingFilePath.size()) + " .");
    }
    return fingFilePath[t];
}

bool Bunch::operator==(const Bunch &bunchIn) const {

    if (fingFilePath.size() != bunchIn.fingFilePath.size())
        return false;

    vector<string> right;
    vector<string> left;
    for (auto val :fingFilePath) {
        left.push_back(val->path());
    }
    for (auto val2 :bunchIn.fingFilePath) {
        right.push_back(val2->path());
    }
    for (auto compare:left) {
        if (find(right.cbegin(), right.cend(), compare) == right.cend())
            return false;
    }

    return true;
    /*
    for (auto compare:bunchIn.fingFilePath) {
        bool check=true;
        for (auto val : fingFilePath) {
            if (compare == val) {
                check = false;
                break;
            }
        }
        if (!check){
            return false;
        }
    }

    return true;*/
}

bool Bunch::operator!=(const Bunch &bunchIn) const {
    return !(*this == bunchIn);
}


Bunch Bunch::operator+(const Bunch &bunchIn) const {

    Bunch newBunchP(pathIn);
    for (auto val2 :bunchIn.fingFilePath) {
        bool check = true;
        for (auto val : fingFilePath) {
            if ((val->path()) == (val2->path())) {
                check = false;
                break;
            }
        }
        if (check) {
            newBunchP.fingFilePath.push_back(val2);
        }
    }

    return newBunchP;
}

Bunch Bunch::operator-(const Bunch &bunchIn) const {
    Bunch newBunchM(pathIn);
    newBunchM.fingFilePath.clear();

    for (auto val :fingFilePath) {
        bool check = true;
        for (auto val2 : bunchIn.fingFilePath) {
            if ((val->path()) == (val2->path())) {
                check = false;
                break;
            }
        }
        if (check) {
            newBunchM.fingFilePath.push_back(val);
        }
    }

    return newBunchM;
}

Bunch &Bunch::operator+=(const Bunch &bunchIn) {

    for (auto val2 :bunchIn.fingFilePath) {
        bool check = true;
        for (auto val : fingFilePath) {
            if ((val->path()) == (val2->path())) {
                check = false;
                break;
            }
        }
        if (check) {
            fingFilePath.push_back(val2);
//            delete val2;
        }
    }

    return *this;
}

Bunch &Bunch::operator-=(const Bunch &bunchIn) {
    Bunch newBunchTmp(pathIn);

    for (auto i :newBunchTmp.fingFilePath) {
        delete i;
    }
    newBunchTmp.fingFilePath.clear();

    for (auto val :fingFilePath) {
        bool check = true;
        for (auto val2 : bunchIn.fingFilePath) {
            if ((val->path()) == (val2->path())) {
                check = false;
                break;
            }
        }
        if (check) {
            newBunchTmp.fingFilePath.push_back(val);
//            delete val;
        }
    }
    for (auto j :fingFilePath) {
        delete j;
    }
    fingFilePath.clear();

    for (auto temp:newBunchTmp.fingFilePath) {
        fingFilePath.push_back(temp);
    }

    return *this;
}

Bunch::operator bool() const {
    return !empty();
}

Bunch::iterator Bunch::begin() const {
    return Bunch::iterator(fingFilePath, 0);
}

Bunch::iterator Bunch::end() const {
    return Bunch::iterator(fingFilePath, fingFilePath.size());
}

void Bunch::read_directory(const string name, vector<string> &v, const bool a) {
    struct stat statbuf;
    const char *file;
    file = name.c_str();
    DIR *dirp = opendir(file);
    struct dirent *dp;
    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" Error:" + name + " in read path. ");

    } else {
        if (statbuf.st_mode & S_IFDIR) {
            while ((dp = readdir(dirp)) != NULL) {
                string testDot = dp->d_name;
                if (testDot == "." || testDot == "..") {
                    continue;
                }
                //if is .. .
                if (dp->d_name[0] != '.') {
                    if (dp->d_name[1] != '.') {
                        string newpath = file; //+ dp->d_name;
                        char check = newpath.back();
                        if (check != '/') {
                            newpath = newpath + "/" + dp->d_name;
                        } else {
                            newpath = newpath + dp->d_name;
                        }
                        v.push_back(newpath);
                        struct stat tmpStatbuf;
                        lstat(newpath.c_str(), &tmpStatbuf);
                        if (tmpStatbuf.st_mode & S_IFDIR) {
                            read_directory(newpath.c_str(), v, a);
                        }
                    }
                }
                if (a == true && dp->d_name[0] == '.') {
                    string hiddenF = file; //+ dp->d_name;
                    char checkF = hiddenF.back();
                    if (checkF != '/') {
                        hiddenF = hiddenF + "/" + dp->d_name;
                    } else {
                        hiddenF = hiddenF + dp->d_name;
                    }
                    v.push_back(hiddenF);
                }//end of . ..
            }
            closedir(dirp);
        }
    }
}





