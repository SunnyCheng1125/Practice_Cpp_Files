#include "Symlink.h"

using namespace std;

Symlink::Symlink(const string n) : Fing(n) {

}

string Symlink::type() const {
    return "symlink";
}
