#include "object.h"
#include <iostream>
#include <ctime>
#include <cstdlib>


Foo rvo() {
    std::cout << "create unnamed foo object\n";
    return Foo();
}

Foo nrvo(){
    Foo f;
    std::cout <<"create name foo object\n";
    return f;
}

Foo branch_rvo(){
    auto num = rand() % 10;
    std::cout <<"create unnamed foo object with branch\n";
    if(num > 0) {
        return Foo(num);
    }else {
        return Foo(num * 5);
    }
}

Foo branch_nrvo() {
    auto num = rand() % 10;
    std::cout << "create named foo object with branch\n";
    if(num > 0){
        Foo f1 = Foo(num);
        return f1;
    }else {
        Foo f2 = Foo(num*5);
        return f2;
    }
}

Foo rvo_and_nrvo_chain() {
    auto f = rvo();
    return f;
}

Foo nrvo_and_rvo_chain() {
    return nrvo();
}

Foo return_arg(Foo f){
    return f;
}

Foo rvo_chain3(){
    return Foo();
}

Foo rvo_chain2(){
    return rvo_chain3();
}

Foo rvo_chain1(){
    return rvo_chain2();
}

Foo nrvo_chain2(){
    Foo f = rvo_chain1();
    f.i = 3;
    std::cout << "side-effects:" << f.i << "\n";
    return f;
}

Foo nrvo_chain1(){
    Foo f = nrvo_chain2();
    f.i = 5;
    std::cout << "side-effects:" << f.i << "\n";
    return f;
}


// see https://godbolt.org/z/oMGxjT

int main(){
    std::cout << "RVO: \n";
    Foo f = rvo(); // copy elision

    std::cout << "\nNRVO: \n";
    Foo f1 = nrvo(); // copy elision

    std::cout << "\nbranch-RVO: \n";
    Foo f2 = branch_rvo(); // copy elision

    std::cout << "\nbranch-nrvo: \n";
    Foo f3 = branch_nrvo(); // no copy elision! move instead

    std::cout << "\nrvo-chains: \n";
    Foo ff = rvo_chain1();

    std::cout << "\nnrvo-chains: \n";
    Foo ff1 = nrvo_chain1();

    std::cout << "\nrvo-nrvo-chain: \n";
    Foo f4 = rvo_and_nrvo_chain(); // copy elision

    std::cout << "\nnrvo_and_rvo_chain: \n";
    Foo f5 = nrvo_and_rvo_chain(); // copy elision

    std::cout << "\nreturn_arg: \n";
    Foo f6 = return_arg(Foo());    // no copy elision! move instead

    std::cout << "\nassign not create: \n";
    Foo f7;
    f7 = rvo();                 // no copy elision! move instead

    // Summary: 
    // When return value optimization (rvo, copy elision) will not occur:
    // 1. callee function return more than one variable
    // 2. callee function return one of args
    // 3. target object in caller is assigned instead of created
    //

// output :
//
// create unnamed foo object
//     create foo
//
//     NRVO:
//     create foo
//     create name foo object
//
//     branch-RVO:
//     create unnamed foo object with branch
//     create foo
//
//     branch-nrvo:
//     create named foo object with branch
//     create foo
//     move create foo
//
//     rvo-nrvo-chain:
//     create unnamed foo object
//     create foo
//
//     nrvo_and_rvo_chain:
//     create foo
//     create name foo object
//
//     return_arg:
//     create foo
//     move create foo
//
//     assign not create:
//     create foo
//     create unnamed foo object
//     create foo
//     move assign foo
//
}
