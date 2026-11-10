```cpp run id=records
#include <print>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

enum class Level { debug, info, warn, error };

constexpr std::string_view name(Level level) {
    constexpr std::string_view names[] = {"debug", "info", "warn", "error"};
    return names[static_cast<int>(level)];
}

struct Logger {
    Level min;

    // Writes one line of key=value pairs, with no timestamp. {:?} quotes and escapes strings.
    void log(Level level, std::string_view msg, const std::vector<std::pair<std::string_view, std::string>> &fields = {},
             std::source_location where = std::source_location::current()) const {
        if (level < min) {
            return;
        }
        std::print("level={} msg={:?} line={}", name(level), msg, where.line());
        for (const auto &[key, value] : fields) {
            std::print(" {}={:?}", key, value);
        }
        std::println("");
    }
};

int main() {
    Logger log{Level::info};
    log.log(Level::debug, "cart loaded", {{"items", "3"}}); // below info: dropped
    log.log(Level::info, "order placed", {{"order", std::to_string(1042)}});
    log.log(Level::warn, "stock low", {{"sku", "A-17"}, {"left", std::to_string(2)}});
    std::string user = "bob\nlevel=info msg=\"login ok\"";
    log.log(Level::info, "login failed", {{"user", user}});
}
```

```output for=records
level=info msg="order placed" line=35 order="1042"
level=warn msg="stock low" line=36 sku="A-17" left="2"
level=info msg="login failed" line=38 user="bob\nlevel=info msg=\"login ok\""
```

- Verdict: idiom. The C++ standard library has no logger.
- The `{:?}` format of `std::print` writes a string in quotes, with escapes for a newline and a quote.
- A default argument of type `std::source_location` takes the line of the caller, so each record shows where it came from.
