//#include "Bunch.h"	// Do this first for best testing.
//#include <sys/types.h>	// for getpwnam
//#include <pwd.h>	// for getpwnam
//#include <unistd.h>	// for chdir
//#include <cassert>	// assert
//#include <iostream>	// for cout, cerr
//#include <iomanip>	// for setw
//#include <string>	// What a mystery!
//
//using namespace std;
//
//void dump(string label, const Bunch &b) {
//    cout << label << ":\n";
//    for (size_t i=0; i<b.size(); i++) {
//        const Fing *p = b.entry(i);
//        cout << string(p->perms()) << ' ' << p->path() << '\n';
//    }
//    cout << '\n';
//}
//
//int main() {
//    try {
//        const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
//        if (chdir(home) != 0)			     // go to ~cs253
//            throw "Can’t chdir to "s + home;
//        const Bunch b1("pub/tree/alpha/iota");
//        Bunch b2("pub/tree2/tau");
//        dump("b1", b1);
//        cout<<"test1"<<'\n';
//        b2+=b2;
//        b2 += Bunch("/etc/group");
//
//        cout<<"test2"<<'\n';
//        dump("b2", b2);
//        dump("b1+b2", b1+b2);
//        cout<<"test3"<<'\n';
//        Bunch b3(b2);
//        b3 += b1;
//        b3 += b1;				// Yes, I did it twice.
//        dump("b3 Before minus",b3);
//        b3=(b3 - Bunch("/etc/group"));	// Should have no effect
//        dump("b3 after minus",b3);
//
//        assert(b1.size() == 3);
//        cout<<"b2 size: "<<b2.size()<<'\n';
//        assert(b2.size() == 3);
//        assert((b1+b2).size() == 6);
//        assert(b1+b2 == b3);
//        assert(b1 == b3-b2);
//        assert(b1);
//        assert(b2);
//        assert(b3);
//        assert(!(b1-b1));
//    }
//    catch (string err) {
//        cerr << "ERROR: " << err << '\n';
//    }
//}
////////////////////////////////////////
#include "Bunch.h"	// Do this first for best testing.
#include <sys/types.h>	// for getpwnam
#include <pwd.h>	// for getpwnam
#include <unistd.h>	// for chdir
#include <cassert>	// assert
#include <iostream>	// for cout, cerr
#include <iomanip>	// for setw
#include <string>	// What a mystery!

using namespace std;

void dump(string label, const Bunch &b) {
    cout << label << ":\n";
    for (size_t i=0; i<b.size(); i++) {
        const Fing *p = b.entry(i);
        cout << string(p->perms()) << ' ' << p->path() << '\n';
    }
    cout << '\n';
}

int main() {
    try {
        const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
        if (chdir(home) != 0)			     // go to ~cs253
            throw "Can’t chdir to "s + home;
        const Bunch b1("pub/tree/alpha/iota");
        Bunch b2("pub/tree2/tau");
        dump("b1", b1);
        b2 += Bunch("/etc/group");
        dump("b2", b2);
        dump("b1+b2", b1+b2);
        Bunch b3(b2);
        b3 += b1;
        b3 += b1;				// Yes, I did it twice.
        b3 -= Bunch("/etc/resolv.conf");	// Should have no effect
        dump("b3 ",b3);
        assert(b1.size() == 3);
        assert(b2.size() == 3);
        assert((b1+b2).size() == 6);
        assert(b1+b2 == b3);
        assert(b1 == b3-b2);
        assert(b1);
        assert(b2);
        assert(b3);
        assert(!(b1-b1));
        Bunch b4=b2;
        dump("b4 = b2",b4);
        b4=b4+b1;
        dump("b4 add b1",b4);
        b4=b4-b1;
        dump("b4 minus b1",b4);
        b4+= Bunch("/etc/resolv.conf");
        dump("b4 add conf",b4);
        b4-= Bunch("/etc/resolv.conf");
        dump("b4 minus conf",b4);

        cout<<"PASS"<<'\n';
    }
    catch (string err) {
        cerr << "ERROR: " << err << '\n';
    }
}