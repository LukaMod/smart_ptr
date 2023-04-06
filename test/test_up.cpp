#include "unique_ptr.h"

#include <iostream>
#include <cassert>

struct Struct {
    int a;
    int b;
};

struct Construct {
    Construct(int a, int b):
        a_(a), b_(b) {}

    int a_;
    int b_;
};

struct Base {
    virtual ~Base() { std::cout << "~Base()" << std::endl; }
    virtual int foo() const { return 0; }
};

struct Derived: public Base {
    virtual ~Derived() { std::cout << "~Derived()" << std::endl; }
    int foo() const override { return 1; }
};

int main() {
    // test make_unique
    xy::unique_ptr<Struct> pStruct = xy::make_unique<Struct>(10, 20);
    assert(pStruct->a == 10 && pStruct->b == 20);

    xy::unique_ptr<Struct> pS2 = xy::make_unique<Struct>(100);
    assert(pS2->a == 100 && pS2->b == 0);

    xy::unique_ptr<Construct> pConstruct = xy::make_unique<Construct>(1, 2);
    assert(pConstruct->a_ == 1 && pConstruct->b_ == 2);

    xy::unique_ptr<int[]> pArray = xy::make_unique<int[]>(10);
    for (size_t i = 0; i < 10; ++i)
        pArray[i] = i;

    // test constructor/get/operator*
    int* p = new int(10);
    xy::unique_ptr<int> up(p);
    assert(up.get() == p && *up == *p);

    xy::unique_ptr<int> up_null;
    assert(up_null == nullptr);

    // test operator=(&&)
    up_null = std::move(up);
    assert(up_null && *up_null == 10 && !up);

    // test swap
    up_null.swap(up);
    assert(up && *up == 10 && !up_null);

    // test release
    int* pr = up.release();
    assert(pr == p);
    delete p;

    // test reset
    xy::unique_ptr<int> up2(new int(20));
    up2.reset();
    assert(up2 == nullptr);
    up2.reset(new int(30));
    assert(*up2 == 30);

    // test array
    int* parr = new int[10]{};
    xy::unique_ptr<int[]> up_arr(parr);

    // test inheritance
    xy::unique_ptr<Base> pB(new Base());
    xy::unique_ptr<Base> pD(new Derived());
    assert(pB->foo() == 0 && pD->foo() == 1);

    return 0;
}