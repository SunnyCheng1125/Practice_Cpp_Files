#include "Symlink.h"

using namespace std;

Symlink::Symlink(const string n) : Fing(n) {

}

Symlink::~Symlink() {

}

string Symlink::type() const {
    return "symlink";
}