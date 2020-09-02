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

void get_formatType(const string argv, const string format, const int cF, const string path, const int cM,
                    const string proName);

void read_directory(const char *name, vector<string> &v, int cA, const string proName);

int main(int argc, char *argv[]) {

    string program_name = argv[0];

    if (argc < 2) {

        cerr << program_name << " [" << program_name << " ] Error: Give more arguments!" << '\n';

    } else {

        int opt;
        int countM = 0, countF = 0, countA = 0;
        string optFormat, optPath;

        //read argv[1] to get option
        while ((opt = getopt(argc, argv, "af:m:")) != -1) {
            switch (opt) {
                case 'm':
                    countM = countM + 1;
                    optPath = optarg;
                    break;

                case 'a':
                    countA = countA + 1;
                    break;

                case 'f':
                    countF = countF + 1;
                    optFormat = optarg;
                    break;

                case ':':
                    cerr << program_name << "Error : Without instruction after the option." << endl;
                    break;

                case '?':
                    cerr << program_name << "Error : Unknown option." << endl;
                    break;

                default:
                    cerr << program_name << " Error: Wrong option " << endl;
                    exit(1);
            }
        }
        if (countM > 1 || countF > 1) {
            cerr << program_name << "Error: option -f or -m should only show once." << endl;
        } else {
            for (int afterOpt = optind; afterOpt < argc; afterOpt++) {
                vector<string> filePath;
                filePath.push_back(argv[afterOpt]);//current path
                read_directory(argv[afterOpt], filePath, countA, program_name);

                for (size_t val = 0; val < filePath.size(); val++) {
                    get_formatType(filePath[val], optFormat, countF, optPath, countM, program_name);
                }
            }
        }
    }
    return 0;
}

void get_formatType(const string argvS, const string format, const int cF, const string path, const int cM,
                    const string proName) {
    struct stat statbuf;
    struct passwd *pwu;
    struct group *pwg;
    string s;
    if (cF > 0) {
        s = format;
    } else {
        s = "%p %U %G %s %n";
    }
    vector<char> argv1(s.begin(), s.end());

    string newS;
    string errorS;
    string permission;
    const char *argv;
    argv = argvS.c_str();
    int result = lstat(argv, &statbuf);

    if (result != 0) {
        cerr << proName << " lstat file path: " << argv << " failed\n";
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
                cerr << proName << " : Undifined " << argv << " file!!";
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
        ifstream in(argv);
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
        if (cM > 0) {
            ifstream nTypein(path);
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

                        m1 = (mediaLineTmp[n + 1] - 87) * 16;

                    } else if (mediaLineTmp[n + 1] >= '0' && mediaLineTmp[n + 1] <= '9') {

                        m1 = (mediaLineTmp[n + 1] - 48) * 16;

                    } else {

                        mediaLine += mediaLineTmp[n];
                        continue;

                    }
                    //change to the actual characters
                    if (mediaLineTmp[n + 2] >= 'a' && mediaLineTmp[n + 2] <= 'f') {

                        m2 = (mediaLineTmp[n + 2] - 87);
                        mtmp = static_cast<char>(m1 + m2);
                        mediaLine += mtmp;

                    } else if (mediaLineTmp[n + 2] >= '0' && mediaLineTmp[n + 2] <= '9') {

                        m2 = (mediaLineTmp[n + 2] - 48);
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

                    y1 = (fStore[y + 1] - 87) * 16;

                } else if (fStore[y + 1] >= '0' && fStore[y + 1] <= '9') {

                    y1 = (fStore[y + 1] - 48) * 16;

                } else {
                    fileLine += fStore[y];
                    continue;
                }
                //change to the actual characters
                if (fStore[y + 2] >= 'a' && fStore[y + 2] <= 'f') {
                    y2 = (fStore[y + 2] - 87);
                    tmp = static_cast<char>(y1 + y2);
                    fileLine += tmp;

                } else if (fStore[y + 2] >= '0' && fStore[y + 2] <= '9') {
                    y2 = (fStore[y + 2] - 48);
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
                        newS += argv;
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
                                if (in.peek() == std::ifstream::traits_type::eof()) {
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
                                    cerr << " " << proName << ": " << argv
                                         << " : An unspecified string ";
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
        cout << newS << '\n';

        if (errorS.empty()) {
//                continue;
        } else {
            cerr << " " << proName << ": " << errorS << "is an undefined behavior" << '\n';
        }
    }
}

void read_directory(const char *name, vector<string> &v, int cA, const string proName) {
    struct stat statbuf;
    DIR *dirp = opendir(name);
    struct dirent *dp;
    int result = lstat(name, &statbuf);
    if (result != 0) {
        cerr << proName << " Error in read directory. " << '\n';

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
                        string newpath = name; //+ dp->d_name;
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
                            read_directory(newpath.c_str(), v, cA, proName);
                        }
                    }
                }
                if (cA > 0 && dp->d_name[0] == '.') {
                    string hiddenF = name; //+ dp->d_name;
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
