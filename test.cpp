#include "vtable.hpp"
#include <iostream>
using std::cout;
using std::endl;

class A{
    public:
        virtual void f() { cout << "A::f()" << endl; }
        virtual void g() { cout << "A::g()" << endl; }
        virtual void h() { cout << "A::h()" << endl; }
        virtual ~A() { cout << "A::~A()" << endl; }
};

class B: public A{
    public:
        virtual void f() { cout << "B::f()" << endl; }
        virtual void g() { cout << "B::g()" << endl; }
        // virtual void h() { cout << "B::h()" << endl; }
        virtual ~B() { cout << "B::~B()" << endl; }
};


#include <map>
#include <string>
#include "D:\Code\C++\mem_check.hpp"

int main() {

    A* a = new A();
    A* b = new B();

    vtable va(a,5); // 3 for 3 virtual functions and 2 for the destructor
    vtable vb(b,5);

    std::cout << "vtable of A:\t" << va << std::endl;
    std::cout << "vtable of B:\t" << vb << std::endl;

    std::cout << "\nCall f()\n";
    a->f();             // call A::f()
    (va[0])();   // call the first virtual function of a
    b->f();             // call B::f()
    (vb[0])();   // call the first virtual function of b

    std::cout << "\nCall g()\n";
    a->g();             // call A::g()
    (va[1])();   // call the second virtual function of a
    b->g();             // call B::g()
    (vb[1])();   // call the second virtual function of b


    std::cout << "\nCall h()\n";
    a->h();             // call A::h()
    (va[2])();   // call the third virtual function of a
    b->h();             // call B::h() which is actually A::h()
    (vb[2])();   // call the third virtual function of b

    std::cout << "\nOverwrite b's vtable with a\n";
    va.overwrite(vb);
    std::cout << "vtable of A:\t" << va << std::endl;
    std::cout << "vtable of B:\t" << vb << std::endl;

    std::cout << "\nCall f()\n";
    a->f();             // call B::f()
    (va[0])();   // call the first virtual function of a
    b->f();             // call B::f()
    (vb[0])();   // call the first virtual function of b
    
    std::cout << "\nOverwritten successfully\n";

    std::cout << "\nDestroy a\n";
    delete a;
    std::cout << "\nDestroy b\n";
    delete b;
    std::cout << "\nSince the vtable was overwritten by a, B::~B() won't be called\n";

}
