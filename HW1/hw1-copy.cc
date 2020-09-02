#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {

    struct stat statbuf;

    if (argc < 2) {
        cerr << "Error: Give more arguments!" << '\n';
    } else {
        for (int i = 1; i < argc; i++) {
            int result = lstat(argv[i], &statbuf);
            if (result != 0) {
                cerr << "lstat file path: " << argv[i] << " failed\n";
                continue;
            }
            switch (statbuf.st_mode & S_IFMT) {
                case S_IFREG:
                    cout << "-";
                    break;

                case S_IFDIR:
                    cout << "d";
                    break;

                case S_IFLNK:
                    cout << "l";
                    break;

                default:
                    cerr << "Undifined " << argv[i] << " file!!";
                    break;
            }
            if (statbuf.st_mode & S_IRUSR) {
                cout << "r";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IWUSR) {
                cout << "w";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IXUSR) {
                cout << "x";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IRGRP) {
                cout << "r";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IWGRP) {
                cout << "w";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IXGRP) {
                cout << "x";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IROTH) {
                cout << "r";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IWOTH) {
                cout << "w";
            } else {
                cout << "-";
            }
            if (statbuf.st_mode & S_IXOTH) {
                cout << "x";
            } else {
                cout << "-";
            }

            time_t now = statbuf.st_mtime;
            auto timevals = localtime(&now);
            char buf[32];
            strftime(buf, sizeof(buf), "%FT%T", timevals);

            cout << "gid: " << dec << statbuf.st_gid << '\n';
            cout << " " << dec << statbuf.st_size;
            cout << " " << buf;
            cout << " " << argv[i] << '\n';

        }
    }
    return 0;
}
