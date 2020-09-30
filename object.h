#include <iostream>

class Foo {
public:
    Foo(int num = 0) : i(num)
    {
        std::cout << "create foo\n";
    }
    Foo(const Foo& f)
    {
        std::cout << "copy create foo\n";
    }
    Foo(Foo&& f)
    {
        std::cout << "move create foo\n";
    }
    Foo& operator=(const Foo& f)
    {
        std::cout << "copy assign foo\n";
        return *this;
    }
    Foo& operator=(Foo&& f)
    {
        std::cout << "move assign foo\n";
        return *this;
    }

    int i;
};
