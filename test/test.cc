#include <iostream>

#include "demangle.h"

namespace test
{

    struct Type
    {
    };

} // namespace test

int main()
{
    std::cout << demangled<test::Type> << std::endl;
}
