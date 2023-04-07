# smart_ptr
unique_ptr &amp;&amp; make_unqiue implemented in C++ 11

### The usage is similar to STL.
```cpp
#include "unique_ptr.h"
#include <iostream>
#include <cassert>

struct Base {
    virtual ~Base() { std::cout << "~Base()" << std::endl; }
    virtual int foo() const { return 0; }
};

struct Derived: public Base {
    virtual ~Derived() { std::cout << "~Derived()" << std::endl; }
    int foo() const override { return 1; }
};

int main() {
    // test constructor/get/operator*
    int* p = new int(10);
    xy::unique_ptr<int> up(p);
    assert(up.get() == p && *up == *p);
    
    xy::unique_ptr<int[]> pArray = xy::make_unique<int[]>(10);

    // test inheritance
    xy::unique_ptr<Base> pB(new Base());
    xy::unique_ptr<Base> pD(new Derived());
    assert(pB->foo() == 0 && pD->foo() == 1);
    
    return 0;
}
```

### xy::make_unique support initializing C struct with parameters
```cpp
#include "unique_ptr.h"
#include <cassert>

struct Struct {
    int a;
    int b;
};

int main() {
  auto pS1 = xy::make_unique<Struct>(10, 20);
  assert(pS1->a == 10 && pS1->b == 20);
  
  auto pS2 = xy::make_unique<Struct>(3);
  assert(pS2->a == 3 && pS2->b == 0);
  
  return 0;
}
```
