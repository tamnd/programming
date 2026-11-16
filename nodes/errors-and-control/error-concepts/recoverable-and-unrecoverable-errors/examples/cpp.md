```cpp run id=recoverable-cpp
#include <charconv>
#include <cstdlib>
#include <iostream>
#include <string>

int port_or_default(const std::string& text, int fallback, std::string& note) {
    // A bad fallback is a bug in the caller: stop the program.
    if (fallback < 1 || fallback > 65535) {
        std::abort();
    }
    // Bad text is an expected error: recover.
    int n = 0;
    auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), n);
    if (ec != std::errc() || end != text.data() + text.size()) {
        note = " (not a number)";
        return fallback;
    }
    note = "";
    return n;
}

int main() {
    for (std::string text : {"8080", "x"}) {
        std::string note;
        int port = port_or_default(text, 80, note);
        std::cout << text << " -> " << port << note << "\n";
    }
}
```

```output for=recoverable-cpp
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: idiom. C++ has no separate mechanism for bugs. The code reports the expected error with the `std::errc` value of `std::from_chars`, and calls `std::abort` for the bug. An exception that no handler catches also ends the program, through `std::terminate`.
