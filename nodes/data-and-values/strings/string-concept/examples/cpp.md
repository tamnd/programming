```cpp run id=std-string-and-string-view
#include <iostream>
#include <string>
#include <string_view>

using namespace std::string_literals;

int main() {
    // std::string owns a mutable sequence of char.
    std::string s = "cat";
    s[0] = 'b';
    s += "s";
    std::cout << s << ' ' << s.size() << '\n';

    // size() counts char values, which are UTF-8 bytes here.
    std::string cafe = "café";
    std::cout << cafe.size() << '\n';

    // A std::string stores its length, so it can hold a null character.
    std::string with_null = "ab\0cd"s;
    std::cout << with_null.size() << ' ' << std::string("ab\0cd").size() << '\n';

    // std::string_view refers to characters that another object owns.
    std::string_view view = s;
    view.remove_prefix(1);
    std::cout << view << ' ' << s << '\n';

    // c_str gives a null-terminated array for C functions.
    std::cout << (s.c_str()[s.size()] == '\0') << '\n';
}
```

```output for=std-string-and-string-view
bats 4
5
5 2
ats bats
1
```

- Verdict: stdlib. `std::string` is a class of the standard library, and it is mutable. `size()` counts `char` values, which are 5 UTF-8 bytes for `"café"`. The literal `"ab\0cd"s` makes a string of 5 characters with a null character inside, but a conversion from a C string stops at the null character. A `std::string_view` refers to the characters of `s` without a copy. `c_str()` ends with a null character.
