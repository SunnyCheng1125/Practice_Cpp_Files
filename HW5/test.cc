#include "Bunch.h"    // Do this first for best testing.
#include <sys/types.h>    // for getpwnam
#include <pwd.h>    // for getpwnam
#include <unistd.h>    // for chdir
#include <iostream>    // for cout, cerr
#include <iomanip>    // for setw
#include <string>    // What a mystery!

using namespace std;

int main() {
    // Yeah, this code is arguably a bit const-happy.
    try {
        const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
        if (chdir(home) != 0)                 // go to ~cs253
            throw "Can’t chdir to "s + home;
        Bunch b("pub/tree");
        for (size_t i=0; i<b.size(); i++) {
            const auto e = b.entry(i);             // a Fing
            const uid_t numeric_user = e.uid();         // force numeric
            const string string_group = e.gid();     // force string
            cout << string(e.perms())             // string permissions
                 << setw(6) << numeric_user         // numeric user id
                 << setw(6) << string_group         // string group id
                 << setw(6) << e.size()             // size is always numeric
                 << ' ' << string(e.mtime())         // string mod time
                 << ' ' << e.path() << '\n';         // path is always string
        }
        cout<<b.size()<<" "<<b.empty()<<'\n';
        cout<<"--------------------------------------------\n";
        b.all();
        for (size_t i=0; i<b.size(); i++) {
            const auto e = b.entry(i);             // a Fing
            const uid_t numeric_user = e.uid();         // force numeric
            const string string_group = e.gid();     // force string
            cout << string(e.perms())             // string permissions
            << setw(6) << numeric_user         // numeric user id
            << setw(6) << string_group         // string group id
            << setw(6) << e.size()             // size is always numeric
            << ' ' << string(e.mtime())         // string mod time
            << ' ' << e.path() << '\n';         // path is always string
        }
        cout<<b.size()<<" "<<b.empty()<<'\n';
        cout<<"--------------------------------------------\n";
        b.path("pub/tree/alpha");
        for (size_t i=0; i<b.size(); i++) {
            const auto e = b.entry(i);             // a Fing
            const uid_t numeric_user = e.uid();         // force numeric
            const string string_group = e.gid();     // force string
            cout << string(e.perms())             // string permissions
                 << setw(6) << numeric_user         // numeric user id
                 << setw(6) << string_group         // string group id
                 << setw(6) << e.size()             // size is always numeric
                 << ' ' << string(e.mtime())         // string mod time
                 << ' ' << e.path() << '\n';         // path is always string
        }
        cout<<b.size()<<" "<<b.empty()<<'\n';
        cout<<"---------------------------------------------\n";
        Bunch bc("pub/tree/alpha/iota");
        for (size_t i=0; i<bc.size(); i++) {
            const auto e = bc.entry(100);             // a Fing
            const uid_t numeric_user = e.uid();         // force numeric
            const string string_group = e.gid();     // force string
            cout << string(e.perms())             // string permissions
                 << setw(6) << numeric_user         // numeric user id
                 << setw(6) << string_group         // string group id
                 << setw(6) << e.size()             // size is always numeric
                 << ' ' << string(e.mtime())         // string mod time
                 << ' ' << e.path() << '\n';         // path is always string
        }
        cout<<bc.size()<<" "<<bc.empty()<<'\n';
    }
    catch (string err) {
        cerr << "ERROR: " << err << '\n';
    }
}
