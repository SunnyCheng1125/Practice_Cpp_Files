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

Bunch::Bunch(const string pAth, const string mAgic, const string fOrmat, const bool aLl) {
    pathIn = pAth;
    magicIn = mAgic;
    formatIn = fOrmat;
    allIn = aLl;
    filePath = filePath;
    newFilePath = newFilePath;

    filePath.push_back(pAth);//current path
    read_directory(pAth, filePath, aLl);
    for (size_t val = 0; val < filePath.size(); val++) {
        get_formatType(filePath[val], magicIn, formatIn);
    }
}

Bunch::Bunch(const Bunch &bunchC) {
    pathIn = bunchC.pathIn;
    magicIn = bunchC.magicIn;
    formatIn = bunchC.formatIn;
    allIn = bunchC.allIn;
    filePath = bunchC.filePath;
    newFilePath = bunchC.newFilePath;
}

Bunch &Bunch::operator=(const Bunch &bunchA) {

    pathIn = bunchA.pathIn;
    magicIn = bunchA.magicIn;
    formatIn = bunchA.formatIn;
    allIn = bunchA.allIn;
    filePath = bunchA.filePath;
    newFilePath = bunchA.newFilePath;

    return *this;
}

Bunch::~Bunch() {

}

void Bunch::path(string path) {
    newFilePath.clear();
    pathIn = path;
//    read_directory(pathIn, filePath, allIn);
    for (size_t val = 0; val < filePath.size(); val++) {
        get_formatType(filePath[val], magicIn, formatIn);
    }
}

void Bunch::magic(string magic) {
    newFilePath.clear();
    magicIn = magic;
//    read_directory(pathIn, filePath, allIn);
    for (size_t val = 0; val < filePath.size(); val++) {
        get_formatType(filePath[val], magicIn, formatIn);
    }
}

void Bunch::format(string format) {
    newFilePath.clear();
    formatIn = format;
//    read_directory(pathIn, filePath, allIn);
    for (size_t val = 0; val < filePath.size(); val++) {
        get_formatType(filePath[val], magicIn, formatIn);
    }
}

void Bunch::all(bool a) {
    newFilePath.clear();
    filePath.clear();
    filePath.push_back(pathIn);
    allIn = a;
    read_directory(pathIn, filePath, a);
    for (size_t val = 0; val < filePath.size(); val++) {
        get_formatType(filePath[val], magicIn, formatIn);
    }
}

size_t Bunch::size() {
    return filePath.size();
}

//Return true iff this Bunch has no entries.
bool Bunch::empty() {
    if (filePath.size() == 1)
        return true;
    else
        return false;
}

string Bunch::entry(size_t t) {
    string S;
    S = newFilePath[t];
    return S;
}

void Bunch::get_formatType(const string argvS, const string magic, const string format) {
    struct stat statbuf;
    struct passwd *pwu;
    struct group *pwg;
    string s;
    s = format;
    vector<char> argv1(s.begin(), s.end());
    string newS;
    string errorS;
    string permission;
    const char *file;
    file = argvS.c_str();

    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" lstat file path: " + argvS + " failed");

    } else {
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
                throw string(" : Undifined " + argvS + " file!");
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
        //end of permission section

        //user name
        pwu = getpwuid(statbuf.st_uid);
        //group name
        pwg = getgrgid(statbuf.st_gid);

        //define the media type
        //read argv[i] first line
        ifstream in(file);
        char firstline;
        char tmp;
        int y1, y2;
        vector<char> fStore;//32 char

        //read 32 char
        for (int x = 0; x < 32; x++) {
            if (in.get(firstline)) {
                fStore.push_back(firstline);
            } else {
                break;
            }
        }
        //read media-types as string one by one
        string getLine;//push back to media0
        string mediaLine;//lines after process
        string mediaLineTmp;//lines of media0 before transition
        vector<string> mediaO; //the original values of the media-types file
        vector<string> media;//the new values

        if (magic != "/pub/media-types") {
            ifstream nTypein(magic);
            while (nTypein >> getLine) {
                mediaO.push_back(getLine);
            }

        } else {
            const string home = getpwnam("cs253")->pw_dir;
            ifstream dTypein(home + "/pub/media-types");
            while (dTypein >> getLine) {
                mediaO.push_back(getLine);
            }
        }
        int m1, m2;
        char mtmp;
        for (size_t m = 0; m < mediaO.size(); m++) {
            mediaLineTmp = mediaO[m];
            for (size_t n = 0; n < mediaLineTmp.size(); n++) {
                if (mediaLineTmp[n] == '%') {

                    //change to the actual characters
                    if (mediaLineTmp[n + 1] >= 'a' && mediaLineTmp[m + 1] <= 'f') {

                        m1 = (mediaLineTmp[n + 1] - 'a' + 10) * 16;

                    } else if (mediaLineTmp[n + 1] >= '0' && mediaLineTmp[n + 1] <= '9') {

                        m1 = (mediaLineTmp[n + 1] - '0') * 16;

                    } else {

                        mediaLine += mediaLineTmp[n];
                        continue;

                    }
                    //change to the actual characters
                    if (mediaLineTmp[n + 2] >= 'a' && mediaLineTmp[n + 2] <= 'f') {

                        m2 = (mediaLineTmp[n + 2] - 'a' + 10);
                        mtmp = static_cast<char>(m1 + m2);
                        mediaLine += mtmp;

                    } else if (mediaLineTmp[n + 2] >= '0' && mediaLineTmp[n + 2] <= '9') {

                        m2 = (mediaLineTmp[n + 2] - '0');
                        mtmp = static_cast<char>(m1 + m2);
                        mediaLine += mtmp;

                    } else {

                        mediaLine += mediaLineTmp[n];
                        continue;
                    }
                    n++;
                    n++;
                } else {
                    mediaLine += mediaLineTmp[n];
                }
            }
            media.push_back(mediaLine);
            mediaLine = "";
        }//end of process of media types

        //argv[i] smegic number process
        //replace chars that after %sign
        string fileLine;//new first line of the file, sting type
        for (size_t y = 0; y < fStore.size(); y++) {
            if (fStore[y] == '%') {
                //change to the actual characters
                if (fStore[y + 1] >= 'a' && fStore[y + 1] <= 'f') {

                    y1 = (fStore[y + 1] - 'a' + 10) * 16;

                } else if (fStore[y + 1] >= '0' && fStore[y + 1] <= '9') {

                    y1 = (fStore[y + 1] - '0') * 16;

                } else {
                    fileLine += fStore[y];
                    continue;
                }
                //change to the actual characters
                if (fStore[y + 2] >= 'a' && fStore[y + 2] <= 'f') {
                    y2 = (fStore[y + 2] - 'a' + 10);
                    tmp = static_cast<char>(y1 + y2);
                    fileLine += tmp;

                } else if (fStore[y + 2] >= '0' && fStore[y + 2] <= '9') {
                    y2 = (fStore[y + 2] - '0');
                    tmp = static_cast<char>(y1 + y2);
                    fileLine += tmp;

                } else {
                    fileLine += fStore[y];
                    continue;
                }
                y++;
                y++;
            } else {
                fileLine += fStore[y];
            }
        }//end of argv file procession

        //final,the output of the format
        //define the output format
        for (size_t j = 0; j < argv1.size(); j++) {
            //access time
            time_t a = statbuf.st_atime;
            auto atimevals = localtime(&a);
            char abuf[32];
            strftime(abuf, sizeof(abuf), "%FT%T", atimevals);

            //modified time
            time_t m = statbuf.st_mtime;
            auto mtimevals = localtime(&m);
            char mbuf[32];
            strftime(mbuf, sizeof(mbuf), "%FT%T", mtimevals);

            //changed time
            time_t c = statbuf.st_ctime;
            auto ctimevals = localtime(&c);
            char cbuf[32];
            strftime(cbuf, sizeof(cbuf), "%FT%T", ctimevals);
            //search for % sign and replace it
            if (argv1[j] == '%') {
                j++;
                switch (argv1[j]) {
                    case 'n': {
                        newS += file;
                        break;
                    }
                    case 'p': {
                        newS += permission;
                        break;
                    }
                    case 'u': {
                        newS += std::to_string(statbuf.st_uid);
                        break;
                    }
                    case 'U': {
                        newS += pwu->pw_name;
                        break;
                    }
                    case 'g': {
                        newS += std::to_string(statbuf.st_gid);
                        break;
                    }
                    case 'G': {
                        newS += pwg->gr_name;
                        break;
                    }
                    case 's': {
                        newS += std::to_string(statbuf.st_size);
                        break;
                    }
                    case 'a': {
                        newS += abuf;
                        break;
                    }
                    case 'm': {
                        newS += mbuf;
                        break;
                    }
                    case 'c': {
                        newS += cbuf;
                        break;
                    }
                    case 'M': {
                        //determine media type of the input argv
                        string readMediaLine;
                        int test = 0;
                        size_t z;

                        switch (statbuf.st_mode & S_IFMT) {
                            //regular file
                            case S_IFREG:
                                if (statbuf.st_size == 0) {
                                    // Empty File
                                    newS += "inode/empty";

                                } else {
                                    //find if fileLine contains media content
                                    for (z = 0; z < media.size(); z++) {
                                        readMediaLine = media[z];
                                        if (fileLine.compare(0, media[z].size(), readMediaLine) == 0) {
                                            newS += media[z + 1];
                                            test = 1;
                                            break;
                                        } else {
                                            z++;
                                        }
                                    }
                                    if (test == 0) {
                                        newS += "application/octet-data";
                                    }
                                    //end of : find if fileLine contains media content
                                }
                                break;
                                //directory file
                            case S_IFDIR:
                                newS += "inode/directory";
                                break;
                                //link file
                            case S_IFLNK:
                                newS += "inode/symlink";
                                break;
                                //undefined file
                            default:
                                //find if fileLine contains media content
                                for (z = 0; z < media.size(); z++) {
                                    readMediaLine = media[z];
                                    readMediaLine = media[z];
                                    if (fileLine.compare(0, media[z].size(), readMediaLine) == 0) {
                                        newS += media[z + 1];
                                        test = 1;
                                        break;
                                    } else {
                                        z++;
                                    }
                                }
                                if (test == 0) {
                                    throw string(" " + argvS + " : An unspecified string ");
                                }
                                //end of : find if fileLine contains media content
                                break;
                        }
                        //end of determine media type
                        break;
                    }
                    default: {
                        errorS += "%";
                        errorS += argv1[j];
                        errorS += " ";

                        newS += "%";
                        newS += argv1[j];
                        break;
                    }
                }
            } else {
                newS += argv1[j];
            }
        }//end of defining format
        newFilePath.push_back(newS);

        if (errorS.empty()) {
//                continue;
        } else {
            throw string(" " + errorS + "is an undefined behavior");
        }
    }
}

void Bunch::read_directory(const string name, vector<string> &v, const bool a) {
    struct stat statbuf;
    const char *file;
    file = name.c_str();
    DIR *dirp = opendir(file);
    struct dirent *dp;
    int result = lstat(file, &statbuf);

    if (result != 0) {
        throw string(" Error in read path. ");

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
