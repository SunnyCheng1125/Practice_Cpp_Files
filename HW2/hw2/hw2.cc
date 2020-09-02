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

using namespace std;

int main(int argc, char *argv[]) {

    struct stat statbuf;
    struct passwd *pwu;
    struct group *pwg;
    string program_name = argv[0];

    if (argc < 3) {

        if(argc==2)
        {
            cerr << program_name << " [" <<program_name<<" \""<< argv[1] << "\""<<" ] Error: Give more arguments!"<< '\n';
        }
        else
        {
            cerr << program_name << " [" <<program_name<<" ] Error: Give more arguments!"<< '\n';
        }

    } else {

        string s = argv[1];
        vector<char> argv1(s.begin(), s.end());

        for (int i = 2; i < argc; i++) {

            string newS;
            string errorS;
            string permission;

            int result = lstat(argv[i], &statbuf);

            if (result != 0) {

                cerr << program_name << " lstat file path: " << argv[i] << " failed\n";
                continue;

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
                        cerr << program_name << " : Undifined " << argv[i] << " file!!";
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

                //user name
                pwu = getpwuid(statbuf.st_uid);

                //group name
                pwg = getgrgid(statbuf.st_gid);


                //define the media type
                ifstream in(argv[i]);
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
                const string home = getpwnam("cs253")->pw_dir;
                ifstream typein(home + "/pub/media-types");
                string getLine;
                string mediaLine;
                string mediaLineTmp;
                vector<string> mediaO; //the original values of the media-types file
                vector<string> media;


                while (typein >> getLine) {

                    mediaO.push_back(getLine);
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

//                                mtmp = m1 + m2;
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
//                    cout<<"mediaLine: "<<mediaLine<<'\n';
                    mediaLine = "";
                }

                //replace chars that after %sign
                string fileLineTmp;//new first line of the file, sting type
//                vector<string> fileLine;
                for (size_t y = 0; y < fStore.size(); y++) {

                    if (fStore[y] == '%') {

                        //change to the actual characters
                        if (fStore[y + 1] >= 'a' && fStore[y + 1] <= 'f') {

                            y1 = (fStore[y + 1] - 87) * 16;

                        } else if (fStore[y + 1] >= '0' && fStore[y + 1] <= '9') {

                            y1 = (fStore[y + 1] - 48) * 16;

                        } else {
                            fileLineTmp += fStore[y];
                            continue;
                        }
                        //change to the actual characters
                        if (fStore[y + 2] >= 'a' && fStore[y + 2] <= 'f') {

                            y2 = (fStore[y + 2] - 87);
                            tmp = static_cast<char>(y1 + y2);
                            fileLineTmp += tmp;

                        } else if (fStore[y + 2] >= '0' && fStore[y + 2] <= '9') {

                            y2 = (fStore[y + 2] - 48);
                            tmp = static_cast<char>(y1 + y2);
                            fileLineTmp += tmp;

                        } else {

                            fileLineTmp += fStore[y];
                            continue;
                        }
                        y++;
                        y++;
                    } else {
                        fileLineTmp += fStore[y];
                    }
                }
//                cout<<"fileline tmp "<<fileLineTmp<<'\n';

//                fileLine.push_back(fileLineTmp);

                //final,the output of the format
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

                                newS += argv[i];
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
                                size_t position;
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
                                                position = fileLineTmp.find(readMediaLine);

                                                if (position != fileLineTmp.npos) {
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
                                            position = fileLineTmp.find(readMediaLine);

                                            if (position != fileLineTmp.npos) {
                                                newS += media[z + 1];
                                                test = 1;
                                                break;

                                            } else {
                                                z++;
                                            }
                                        }
                                        if (test == 0) {
                                            cerr << " " << program_name << ": " << argv[i]
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
                }
                cout << newS << '\n';

                if (errorS.empty()) {
                    continue;
                } else {
                    cerr << " " << program_name << ": " << errorS << "is an undefined behavior" << '\n';
                }
            }
        }
    }
    return 0;
}