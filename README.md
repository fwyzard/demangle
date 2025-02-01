C++20 compile-time demangled type name, based on

  https://www.reddit.com/r/cpp/comments/lfi6jt/finally_a_possibly_portable_way_to_convert_types/

and

  https://rodusek.com/posts/2021/03/09/getting-an-unmangled-type-name-at-compile-time/

with further developments by Andrea Bocci (CERN) and Simeon Ehrig (HZDR).

Usage:
```c++
#include <iostream>

#include <demangle.h>

namespace test {

  struct Type
  {
  };

}  // namespace test

int main() {
  std::cout << demangled<test::Type> << std::endl;
}
```

Compiled with
```bash
$ g++ test/test.cc -std=c++20 -Iinclude -o demangleTest
```

Should print
```bash
$ ./demangleTest
test::Type
```

[Try it on Godbolt](https://godbolt.org/z/jE3jz679d).
