```cpp run id=ports
#include <print>
#include <stdexcept>
#include <string>

struct ConfigError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Counter {
    int& checked;
    ~Counter() { ++checked; }
};

int parse_port(const std::string& text) {
    int port = 0;
    try {
        port = std::stoi(text);
    } catch (const std::invalid_argument&) {
        throw ConfigError("not a number: " + text);
    }
    if (port < 1 || port > 65535) {
        throw ConfigError("out of range: " + std::to_string(port));
    }
    return port;
}

int main() {
    int checked = 0;
    for (const char* text : {"8080", "70000", "abc"}) {
        try {
            Counter counter{checked};
            std::println("port {}", parse_port(text));
        } catch (const ConfigError& e) {
            std::println("error: {}", e.what());
        }
    }
    std::println("checked {}", checked);
}
```

```output
port 8080
error: out of range: 70000
error: not a number: abc
checked 3
```

- Verdict: native. `throw` throws a value of any copyable type, and a `try` block with `catch` handlers catches it by type.
- C++ has no `finally`. A local object's destructor runs when the scope ends, also during stack unwinding, so cleanup lives in a destructor. Here `Counter` counts the values on both paths.
- Throw by value and catch by `const` reference. A catch by value copies the object and slices off the part of a derived class.
- A function declared `noexcept` must not let an exception escape. If one does, the program calls `std::terminate`. A destructor is `noexcept` unless it says otherwise.
- `std::throw_with_nested` and `std::rethrow_if_nested` keep the original exception as a nested exception.
