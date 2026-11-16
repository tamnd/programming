```cpp run id=codes-cpp
#include <charconv>
#include <iostream>
#include <string>
#include <system_error>

enum Code { Ok, Empty, NotANumber, OutOfRange };
const char* names[] = {"ok", "empty", "not a number", "out of range"};

// Returns a code. On success, writes the port.
Code parse_port(const std::string& text, int& port) {
    if (text.empty()) return Empty;
    int value = 0;
    const char* last = text.data() + text.size();
    auto [end, ec] = std::from_chars(text.data(), last, value);
    if (ec == std::errc::invalid_argument || end != last) return NotANumber;
    if (ec == std::errc::result_out_of_range || value < 0 || value > 65535) return OutOfRange;
    port = value;
    return Ok;
}

int main() {
    for (std::string text : {"8080", "", "x", "99999"}) {
        int port = 0;
        Code code = parse_port(text, port);
        if (code != Ok) {
            std::cout << "'" << text << "' -> error code " << code << " (" << names[code] << ")\n";
        } else {
            std::cout << "'" << text << "' -> port " << port << "\n";
        }
    }
}
```

```output for=codes-cpp
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: stdlib. C++ usually throws exceptions, but some standard functions return a code. `std::from_chars` throws nothing; it reports a failure in the `ec` field of its result, as `std::errc::invalid_argument` or `std::errc::result_out_of_range`.
