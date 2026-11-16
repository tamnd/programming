```cpp run id=propagate-cpp
#include <iostream>
#include <stdexcept>
#include <string>

int parse_port(const std::string& text) {
    if (text.empty() || text.find_first_not_of("0123456789") != std::string::npos) {
        throw std::invalid_argument("not a number: " + text);
    }
    return std::stoi(text);
}

int load_config(const std::string& line) {
    std::string value = line.substr(line.find('=') + 1);
    try {
        return parse_port(value);
    } catch (const std::invalid_argument& e) {
        // Add context to the message.
        throw std::runtime_error("config line '" + line + "': " + e.what());
    }
}

int main() {
    for (std::string line : {"port=8080", "port=x"}) {
        try {
            int port = load_config(line);
            std::cout << "port " << port << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "error: " << e.what() << "\n";
        }
    }
}
```

```output for=propagate-cpp
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. An exception goes up the stack to the nearest matching handler, and functions in between need no code for it. Here the new exception copies the old message into its own; it does not keep the original object.
