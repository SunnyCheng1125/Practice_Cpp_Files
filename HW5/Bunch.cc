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

Bunch::Bunch(const string pAth, const bool aLl) {
    pathIn = pAth;
    allIn = aLl;
    filePath = filePath;

    filePath.push_back(pAth);//current path
    read_directory(pAth, filePath, aLl);

    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing(filePath[val]));
    }
}

Bunch::Bunch(const Bunch &bunchC) {
    pathIn = bunchC.pathIn;
    allIn = bunchC.allIn;
    filePath = bunchC.filePath;
}

Bunch &Bunch::operator=(const Bunch &bunchA) {

    pathIn = bunchA.pathIn;
    allIn = bunchA.allIn;
    filePath = bunchA.filePath;

    return *this;
}

Bunch::~Bunch() {

}

void Bunch::path(string path) {
    filePath.clear();
    pathIn = path;
    filePath.push_back(pathIn);
    fingFilePath.clear();
    read_directory(pathIn, filePath, allIn);
    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing(filePath[val]));
    }
}

void Bunch::all(bool a) {
    filePath.clear();
    filePath.push_back(pathIn);
    allIn = a;
    fingFilePath.clear();
    read_directory(pathIn, filePath, allIn);
    for (size_t val = 0; val < filePath.size(); val++) {
        fingFilePath.push_back(Fing(filePath[val]));
    }
}

size_t Bunch::size() const {
    return filePath.size();
}

//Return true iff this Bunch has no entries.
bool Bunch::empty() const {
    if (filePath.size() == 0)
        return true;
    else
        return false;
}

Fing Bunch::entry(size_t t) const { //const?
    if (t >= filePath.size()) {
        string s = to_string(t);
        throw string("Wrong entry size " + s);
    }
    return fingFilePath[t];
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
