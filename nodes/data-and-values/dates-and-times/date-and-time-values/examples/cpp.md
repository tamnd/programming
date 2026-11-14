```cpp run id=chrono-calendar
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // A sys_seconds is a time_point of the system clock: seconds since 1970-01-01 UTC.
    sys_seconds t = sys_days{2024y / March / 9} + 12h;
    std::cout << t << ' ' << t.time_since_epoch().count() << '\n';

    // A year_month_day is a civil date. Plus 1 month can give a date that is not valid.
    year_month_day next = 2024y / January / 31 + months{1};
    std::cout << next.ok() << ' ' << year_month_day{2024y / February / last} << '\n';

    // A duration has a type for its unit.
    minutes m = 90min;
    std::cout << m << ' ' << duration_cast<hours>(m) << ' ' << seconds{m}.count() << '\n';
}
```

```output
2024-03-09 12:00:00 1709985600
0 2024-02-29
90min 1h 5400
```

- Verdict: stdlib. std::chrono has clocks, time_point and duration, and since C++20 calendar types such as year_month_day and time zones with zoned_time. A duration carries its unit in its type, so the compiler checks the conversions.
