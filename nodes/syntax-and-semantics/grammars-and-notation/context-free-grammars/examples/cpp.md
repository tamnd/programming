```cpp run id=most-vexing-parse
#include <iostream>
#include <type_traits>

struct Timer {
    Timer() { std::cout << "a Timer is constructed\n"; }
};

int main() {
    Timer a();  // the grammar reads this as a function declaration
    Timer b{};  // braces always make an object
    std::cout << std::boolalpha;
    std::cout << "a is a function: " << std::is_function_v<decltype(a)> << '\n';
    std::cout << "b is a function: " << std::is_function_v<decltype(b)> << '\n';
}
```

```output for=most-vexing-parse
a Timer is constructed
a is a function: true
b is a function: false
```

- Verdict: library. Boost.Spirit writes EBNF-like grammars as C++ expressions, PEGTL writes PEG grammars as templates, and Bison generates C++ parsers. The grammar of C++ is ambiguous between declarations and expressions, and the rule is that any statement that can be a declaration is one. So `Timer a();` declares a function named `a`, and only `b` is a `Timer`. Braces, as in `Timer b{};`, cannot be read as a declaration of a function.
