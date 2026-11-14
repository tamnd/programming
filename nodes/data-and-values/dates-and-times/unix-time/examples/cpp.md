```cpp run id=system-clock-seconds
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    // A time point of system_clock counts Unix time.
    sys_seconds t{seconds{1709985600}};
    std::cout << std::format("{:%FT%TZ}", t) << '\n';
    std::cout << t.time_since_epoch().count() << ' '
              << time_point_cast<milliseconds>(t).time_since_epoch().count() << '\n';

    // Days since the epoch, and a date with a time turned back into seconds.
    sys_days day = floor<days>(t);
    std::cout << day.time_since_epoch().count() << ' ' << year_month_day{day} << '\n';
    sys_seconds noon = sys_days{year{2024} / March / 9} + 12h;
    std::cout << noon.time_since_epoch().count() << '\n';
}
```

```output
2024-03-09T12:00:00Z
1709985600 1709985600000
19791 2024-03-09
1709985600
```

- Verdict: stdlib. std::chrono::system_clock counts Unix time, and sys_seconds and sys_days are its time points in seconds and days. system_clock::to_time_t and from_time_t convert to and from the C type time_t.
