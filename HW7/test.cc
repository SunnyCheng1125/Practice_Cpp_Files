/*
#include "Bunch.h"    // Do this first for best testing.
#include <sys/types.h>    // for getpwnam
#include <pwd.h>    // for getpwnam
#include <unistd.h>    // for chdir
#include <cassert>    // assert
#include <iostream>    // for cout, cerr
#include <iomanip>    // for setw
#include <string>    // What a mystery!


using namespace std;

void dump(string label, const Bunch &b) {
    cout << label << ":\n";
    for (const Fing *p : b)
        cout << string(p->perms()) << ' ' << p->path() << '\n';
    cout << '\n';
}

int main() {
    try {
        const auto home = getpwnam("cs253")->pw_dir; // cs253’s home dir
        if (chdir(home) != 0)                 // go to ~cs253
            throw "Can’t chdir to "s + home;
        const Bunch b1("pub/tree/alpha/iota");
        Bunch b2("pub/tree2/tau");
        dump("b1", b1);
        b2 += Bunch("/etc/group");
        dump("b2", b2);
        dump("b1+b2", b1 + b2);
        Bunch b3(b2);
        dump("b3(b2)",b3);
        b3 += b1;
        b3 += b1;                // Yes, I did it twice.
        b3 -= Bunch("/etc/resolv.conf");    // Should have no effect
//        dump("no effect b3 ",b3);
//        b3 -= Bunch("pub/tree/alpha/iota");    // Should have no effect
//        dump("effect b3 ",b3);
        dump("dump b3 ",b3);
        cout << "b3:\n";
        for (auto fp : b3)
            cout << string(fp->perms()) << ' ' << fp->path() << '\n';
        // No output is expected after this.  The assertions should all succeed.
        assert(b1.size() == 3);
        assert(b2.size() == 3);
        assert((b1 + b2).size() == 6);

        assert(b1 + b2 == b3);
//        cout << "after b1+b2=b3" << '\n';
//        dump("new b1", b1);
//        dump("new b2", b2);
//        dump("new b3", b3);

//        dump("current b2",b2);
//        dump("current b3",b3);

//        assert(b1 == b3 - b2);
//        cout<<"after b1==b3-b2"<<'\n';
//        dump("b3-b2", b3-b2);
//        dump("new second b1", b1);
//        dump("new second b2", b2);
//        dump("new second b3", b3);

        assert(b1);
        assert(b2);
        assert(b3);
        assert(!(b1 - b1));
        assert(!b1.empty());
        assert((b3 - b3).empty());
//
        Bunch b4 = b2;
        dump("b4 = b2", b4);
        b4 = b4 + b1;
        dump("b4 add b1", b4);
        b4 = b4 - b1;
        dump("b4 minus b1", b4);


        Bunch b5(b4);
        dump("b5", b5);
        
        b4 += Bunch("/etc/resolv.conf");
        dump("b4 add conf", b4);


        dump("b4", b4);
        b4 -= Bunch("/etc/resolv.conf");
        dump("b4 minus conf", b4);
        assert(b4 == b5);

//        cout << "PASS 1" << '\n';
        Bunch::iterator it = b1.begin();
        assert(it != b1.end());
//        cout << "PASS 2" << '\n';
        const Fing *cfp = *it;
//        cout << "PASS 3" << '\n';
        assert(cfp != nullptr);
//        cout << "PASS 4" << '\n';
        assert(cfp->size() > 0);
//        cout << "PASS 5" << '\n';
        assert(b1.size() == 3);
//        cout << "PASS 6" << '\n';
        ++it;
        assert(it != b1.end()); // it now “points” to the second entry
//        cout << "PASS 7" << '\n';
        it++;
        assert(b1.end() != it); // it now “points” to the third entry
//        cout << "PASS 8" << '\n';
        ++it;
        assert(it == b1.end()); // it now “points” PAST the third entry
//        cout << "PASS 9" << '\n';


        const Bunch lotsafiles("pub/lotsafiles");//zstdmt -> zstd

        cout << "PASS 10" << '\n';
        for (const Fing *fp : lotsafiles) {
            //cout<<"PASS 11"<<'\n';
            const string perms = fp->perms(), type = fp->type();
            switch (perms[0]) {
                case 'd':
                    //cout<<"PASS 12"<<'\n';
                    //cout<<"type "<<type<<'\n';
                    //cout<<"perms "<<perms<<'\n';
                    assert(type == "directory");
                    assert(dynamic_cast<const Directory *>(fp));
                    //cout<<"PASS 13"<<'\n';
                    assert(dynamic_cast<const Symlink *>(fp) == nullptr);
                    //cout<<"PASS 14"<<'\n';
                    break;
                case 'l':
                    //cout<<"PASS 15"<<'\n';
                    //cout<<"type "<<type<<'\n';
                    //cout<<"perms "<<perms<<'\n';
                    assert(type == "symlink");
                    assert(dynamic_cast<const Symlink *>(fp));
                    //cout<<"PASS 16"<<'\n';
                    assert(dynamic_cast<const Regular *>(fp) == nullptr);
                    //cout<<"PASS 17"<<'\n';
                    break;
                case '-':
                    //cout<<"PASS 18"<<'\n';
                    assert(type == "regular");
                    assert(dynamic_cast<const Regular *>(fp));
                    //cout<<"PASS 19"<<'\n';
                    assert(dynamic_cast<const Directory *>(fp) == nullptr);
                    //cout<<"PASS 20"<<'\n';
                    break;
            }
        }
        cout << "PASS All" << '\n';
    }
    catch (string err) {
        cerr << "ERROR: " << err << '\n';
    }
}
*/

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
    for (const Fing *p : b)
	cout << string(p->perms()) << ' ' << p->path() << '\n';
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
	cout << "b3:\n";
	for (auto fp : b3)
	    cout << string(fp->perms()) << ' ' << fp->path() << '\n';

	// No output is expected after this.  The assertions should all succeed.
	assert(b1.size() == 3);
	assert(b2.size() == 3);
	assert((b1+b2).size() == 6);
	assert(b1+b2 == b3);
	assert(b1 == b3-b2);
	assert(b1);
	assert(b2);
	assert(b3);
	assert(!(b1-b1));
	assert(!b1.empty());
	assert((b3-b3).empty());

	Bunch::iterator it = b1.begin();
	assert(it != b1.end());
	const Fing *cfp = *it;
	assert(cfp != nullptr);
	assert(cfp->size() > 0);
	assert(b1.size() == 3);
	++it; assert(it != b1.end()); // it now “points” to the second entry
	it++; assert(b1.end() != it); // it now “points” to the third entry
	++it; assert(it == b1.end()); // it now “points” PAST the third entry


	const Bunch lotsafiles("pub/lotsafiles");
	for (const Fing *fp : lotsafiles) {
	    const string perms = fp->perms(), type = fp->type();
	    switch (perms[0]) {
		case 'd':
		    assert(type == "directory");
		    assert(dynamic_cast<const Directory *>(fp));
		    assert(dynamic_cast<const Symlink *>(fp) == nullptr);
		    break;
		case 'l':
		    assert(type == "symlink");
		    assert(dynamic_cast<const Symlink *>(fp));
		    assert(dynamic_cast<const Regular *>(fp) == nullptr);
		    break;
		case '-':
		    assert(type == "regular");
		    assert(dynamic_cast<const Regular *>(fp));
		    assert(dynamic_cast<const Directory *>(fp) == nullptr);
		    break;
	    }
	}
    }
    catch (string err) {
	cerr << "ERROR: " << err << '\n';
    }
}

