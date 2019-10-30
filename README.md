# anytype

## What is anytype?

anytype is a small, header-only C++11 library providing a dynamically-typed, memory-safe object wrapper.

## Can't we already do this with dynamic_cast or boost::any?

Yes, but dynamic_cast is relatively slow and boost::any is a huge dependency. anytype is a small and fast library with an extremely simple interface. anytype has hardly more overhead than a regular heap-allocated object.

## Does this library rely on undefined behavior?

No. We do not delete void pointers or violate strict aliasing in any way.

## How does it work?

Call `.set()` to set the anytype value. Call `.match<typename>(f)` to provide a function `f` that will be called if the anytype's current type is `<typename>`. Pass in a function that takes its argument by reference in order to modify the anytype value. `match` returns `*this` so calls can be chained. Or, pass in no arguments and match simply returns true/false. Call `.value<typename>()` to attempt to extract the value of the anytype. Call `clear()` to delete the stored object.

anytype treats `char const*` as `std::string` for convenience. To store a `char const*`, explicity call `set<char const*>("my c string");`

See this example:

```c++
#include <iostream>
#include <string>

#include "anytype.h"

struct S {
    ~S() {
        std::cout << "S being destructed." << std::endl;
    }
};

int main() {

    anytype a("This is a char const*, but anytype turns it into a std::string");
    a.match<std::string>([](std::string const&) {
        std::cout << "A is a std::string." << std::endl;
    });
    
    /* equivalent:
    if (a.match<std::string>()) {
        std::cout << "A is a std::string." << std::endl;
    }
    */

    a = new S();
    a = 15.8; // ~S() is called

    a.match<float>([](float i) {
        // This will not execute
        std::cout << "A is not a float: " << i << std::endl;
    }).match<double>([](double& i) {
        std::cout << "A is a double: " << i << std::endl;
        i += 5;
    });

    a.value<double>(); // 20.8

    a.set<float>(a.value<double>()); // equivalent: a.set(20.8f);

    a.match<float>([](float i) {
        std::cout << "Now, A is a float: " << i << std::endl;
    });
    
    /* equivalent:
    if (a.match<float>()) {
        std::cout << "Now, A is a float: " << a.value<float>() << std::endl;
    }
    */

    anytype b = a;
    b.match<float>([](float i) {
        std::cout << "B is a copy of A: " << i << std::endl;
    });
    
    
    /* redundant - clear() is automatically called when a and b go out of scope.
    a.clear();
    b.clear();
    */
    
}
```

This outputs the following:

```
A is a std::string.
S being destructed.
A is a double: 15.8
Now, A is a float: 20.8
B is a copy of A: 20.8
```

## Caveats

anytype can not be directly constructed as a pointer, since passing a pointer to anytype's constructor means anytype is taking ownership of a heap object. To actually have anytype store a pointer, call `set` with an explicit pointer type, e.g.

```c++
anytype a;
a.set<my_class*>(my_class_pointer);
```

# LICENSE (MIT)

Copyright &copy; 2018 Chukwudi Okechukwu

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
