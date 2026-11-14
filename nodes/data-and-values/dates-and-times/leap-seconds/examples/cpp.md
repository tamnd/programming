```cpp run id=utc-clock
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    // utc_clock counts the leap seconds; system_clock does not.
    sys_seconds a = sys_days{2016y / 12 / 31} + 23h + 59min + 59s;
    sys_seconds b = sys_days{2017y / 1 / 1};
    utc_seconds ua = clock_cast<utc_clock>(a);
    utc_seconds ub = clock_cast<utc_clock>(b);
    std::cout << (b - a).count() << " " << (ub - ua).count() << "\n";

    // The second between them is the leap second, and format writes it as 60.
    utc_seconds leap = ua + 1s;
    auto info = get_leap_second_info(leap);
    std::cout << std::format("{:%F %T}", leap) << " " << info.is_leap_second << " " << info.elapsed.count() << "\n";

    // TAI is 37 s ahead of UTC since 2017.
    tai_seconds t = clock_cast<tai_clock>(b);
    std::cout << std::format("{:%F %T}", t) << "\n";
    std::cout << get_tzdb().leap_seconds.size() << "\n";
}
```

```output
1 2
2016-12-31 23:59:60 1 27
2017-01-01 00:00:37
27
```

- Verdict: stdlib. Since C++20, `utc_clock` counts leap seconds, `tai_clock` and `gps_clock` count the other scales, and `clock_cast` converts between them with the leap second table of the tz database. `get_leap_second_info` reports whether an instant is in a leap second. `system_clock` does not count them.
