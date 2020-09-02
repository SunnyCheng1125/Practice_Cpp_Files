#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <string.h>

using namespace std;

//int dir_recursive(char *path);
void read_directory(const char *, vector<string> &);

int main(int argc, char *argv[]) {

    string program_name = argv[0];

    if (argc < 2) {

        cerr << program_name << " [" << program_name << " ] Error: Give more arguments!" << '\n';


    } else {
        for (int i = 1; i < argc; i++) {

            vector<string> filePath;
            filePath.push_back(argv[i]);
            read_directory(argv[i], filePath);
            for (auto val:filePath) {
                cout << "file path: " << val << '\n';
            }
        }
    }
    return 0;
}

void read_directory(const char *name, vector<string> &v) {
    struct stat statbuf;
    DIR *dirp = opendir(name);
    struct dirent *dp;
//    cout << "*** test:" << name << '\n';
    int result = lstat(name, &statbuf);

    if (result != 0) {

        cerr<<"Wrong @_@ "<<'\n';

    } else {
            if (statbuf.st_mode & S_IFDIR) {
                while ((dp = readdir(dirp)) != NULL) {

                    //if is .. .
                    if(dp->d_name[0] !='.')
                    {
                        if(dp->d_name[1]!='.'){
                            string newpath = name; //+ dp->d_name;
                            newpath = newpath + "/" + dp->d_name;

                            v.push_back(newpath);

                            struct stat tmpStatbuf;
                            lstat(newpath.c_str(), &tmpStatbuf);
                            if(tmpStatbuf.st_mode & S_IFDIR)
                            {
                                read_directory(newpath.c_str(), v);
                            }
                        }
                    }
                    //end of . ..
                }
                closedir(dirp);

            } else {
                cout<<"file not dir "<<'\n';
                v.push_back(name);
            }
    }
}

