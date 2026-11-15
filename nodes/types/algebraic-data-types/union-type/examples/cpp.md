```cpp run id=either
#include <cctype>
#include <iostream>
#include <string>
#include <variant>

using IntOrText = std::variant<int, std::string>;

std::string describe(const IntOrText& value) {
    if (const int* n = std::get_if<int>(&value)) {
        return "number " + std::to_string(*n) + ", doubled " + std::to_string(*n * 2);
    }
    std::string s = std::get<std::string>(value);
    std::string upper = s;
    for (char& c : upper) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return "text " + s + ", upper " + upper;
}

int main() {
    for (const IntOrText& v : {IntOrText{21}, IntOrText{std::string("ok")}}) {
        std::cout << describe(v) << '\n';
    }
    IntOrText v = std::string("ok");
    try {
        std::cout << std::get<int>(v) << '\n';
    } catch (const std::bad_variant_access&) {
        std::cout << "wrong alternative: bad_variant_access\n";
    }
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
wrong alternative: bad_variant_access
```

- Verdict: native. `std::variant` is a tagged union. It knows which alternative it holds, and `std::get_if` returns a null pointer for the wrong one.
- `std::get` with the wrong alternative throws `std::bad_variant_access`, not undefined behavior.
- A raw `union` also exists, but reading a member that was not written last is undefined behavior.
