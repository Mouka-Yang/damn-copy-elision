#include "object.h"

#include <iostream>

void receiver(Foo f)
{
    std::cout << "receive: " << f.i << "\n";
}

Foo make_foo()
{
    return Foo();
}

class BarWithRef{
public:
    Foo f1;
    Foo f2;
    BarWithRef(const Foo& foo1, const Foo& foo2) : f1(foo1), f2(foo2) {}
};

class BarWithValue {
public:
    Foo f1;
    Foo f2;
    BarWithValue(Foo foo1, Foo foo2) : f1(std::move(foo1)), f2(std::move(foo2)) {}
};

// see https://godbolt.org/z/8dbMTW

int main()
{
    std::cout << "\npass rvalue: \n";
    receiver(Foo());  // copy elision

    std::cout << "\npass lvalue: \n";
    Foo f1;
    receiver(f1);  // no copy elision! use copy

    std::cout << "\npass move(lvalue): \n";
    Foo f2;
    receiver(std::move(f2));  // no copy elision! use move

    std::cout << "\nbuild foo: \n";
    Foo f3 = Foo(Foo(Foo(make_foo())));  // copy elision

    std::cout << "\nObject create tests: \n";
    std::cout << "\ncreate with const ref: \n";
    Foo foo1,foo2;
    std::cout << "pass lvalue: \n";
    BarWithRef b = {foo1,foo2};
    std::cout << "\npass rvalue: \n";
    BarWithRef b2 = {Foo(), Foo()};

    std::cout <<"\ncreate with value: \n";
    std::cout <<"pass lvalue: \n";
    BarWithValue b3 = {foo1,foo2};
    std::cout << "\npass rvalue: \n";
    BarWithValue b4 = {Foo(),Foo()};
}
