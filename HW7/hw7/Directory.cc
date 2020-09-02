#include "Directory.h"

using namespace std;

Directory::Directory(const string n) : Fing(n) {

}

Directory::~Directory() {

}

string Directory::type() const {
    return "directory";
}