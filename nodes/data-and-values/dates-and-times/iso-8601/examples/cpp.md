```cpp run id=chrono-format
#include <chrono>
#include <format>
#include <iostream>
#include <sstream>

int main() {
    using namespace std::chrono;
    sys_seconds t = sys_days{2024y / 12 / 30} + 12h;
    std::cout << std::format("{:%FT%TZ}", t) << "\n";
    std::cout << std::format("{:%G-W%V-%u}", t) << "\n";
    std::cout << std::format("{:%Y-%j}", t) << "\n";

    // %Ez writes the offset in the extended format, with a colon.
    zoned_time z{"Asia/Kolkata", sys_days{2024y / 7 / 9} + 12h + 30min + 5s};
    std::cout << std::format("{:%FT%T%Ez}", z) << "\n";

    // parse reads the same conversions. The offset is subtracted, so the result is UTC.
    std::istringstream in{"2024-07-09T18:00:05+05:30"};
    sys_seconds u;
    in >> parse("%FT%T%Ez", u);
    std::cout << std::format("{:%FT%TZ}", u) << " " << !in.fail() << "\n";
}
```

```output
2024-12-30T12:00:00Z
2025-W01-1
2024-365
2024-07-09T18:00:05+05:30
2024-07-09T12:30:05Z 1
```

- Verdict: stdlib. Since C++20, `std::format` and `std::chrono::parse` use the conversions of `strftime`, plus `%Ez` for an offset with a colon. The library has no parser for ISO 8601 durations.
