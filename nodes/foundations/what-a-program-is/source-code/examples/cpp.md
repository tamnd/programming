```cpp run id=source-location
#include <iostream>
#include <source_location>
#include <string_view>

// A default argument of source_location::current() takes the location of the call.
void check(bool ok, std::string_view what,
           std::source_location loc = std::source_location::current()) {
    std::cout << "line " << loc.line() << ": " << what << " is " << std::boolalpha << ok << '\n';
}

int main() {
    int x = -2;
    check(x > 0, "x > 0");
    check(x * x == 4, "x * x == 4");

    // A raw string literal holds source text with quotes and backslashes as they are.
    std::string_view code = R"(printf("a\tb\n");)";
    std::cout << code << " has " << code.size() << " characters\n";
}
```

```output for=source-location
line 13: x > 0 is false
line 14: x * x == 4 is true
printf("a\tb\n"); has 17 characters
```

- Verdict: native. `std::source_location::current()` as a default argument takes the location of the call, not of the function, so the function needs no macro. Unlike a C macro, it cannot get the text of the argument, so the call passes the text too. A raw string literal keeps quotes and backslashes as they are written, which helps when a program holds source code of another language.
