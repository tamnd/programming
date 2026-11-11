```cpp run id=port
#include <charconv>
#include <expected>
#include <map>
#include <print>
#include <string>
#include <vector>

enum class ConfigError { missing, not_a_number, out_of_range };

std::expected<std::string, ConfigError> lookup(const std::map<std::string, std::string>& config,
                                               const std::string& key) {
    auto it = config.find(key);
    if (it == config.end()) {
        return std::unexpected(ConfigError::missing);
    }
    return it->second;
}

std::expected<int, ConfigError> parse_port(const std::string& text) {
    int n = 0;
    auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), n);
    if (ec != std::errc{} || end != text.data() + text.size()) {
        return std::unexpected(ConfigError::not_a_number);
    }
    if (n < 1 || n > 65535) {
        return std::unexpected(ConfigError::out_of_range);
    }
    return n;
}

int main() {
    std::vector<std::map<std::string, std::string>> configs = {
        {{"port", "8080"}}, {{"port", "x"}}, {{"port", "70000"}}, {}};
    for (const auto& config : configs) {
        auto port = lookup(config, "port").and_then(parse_port);
        if (port) {
            std::println("port {}", *port);
        } else if (port.error() == ConfigError::missing) {
            std::println("use the default 80");
        } else {
            std::println("error code {}", static_cast<int>(port.error()));
        }
    }
}
```

```output
port 8080
error code 1
error code 2
use the default 80
```

- Verdict: stdlib. `std::expected<T, E>` holds either a value of type `T` or an error of type `E`. It is in the standard library from C++23.
- `and_then` calls the next function only when the previous step gave a value. Otherwise it passes the error on, as `?` does in Rust.
- `std::from_chars` is an older form of an error value: it returns a struct with an `std::errc` code and does not throw.
- Reading the value with `*port` when it holds an error is undefined behaviour. The member function `value()` throws `bad_expected_access` in that case.
