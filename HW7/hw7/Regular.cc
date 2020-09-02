#include "Regular.h"

using namespace std;

Regular::Regular(const string n) : Fing(n) {

}

Regular::~Regular() {

}

string Regular::type() const {
    return "regular";
}