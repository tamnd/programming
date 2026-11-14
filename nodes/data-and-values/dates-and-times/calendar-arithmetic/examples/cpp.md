```cpp run id=year-month-day
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    // year_month_day + months moves the month only, and the result can be invalid.
    year_month_day jan31 = 2024y / January / 31;
    year_month_day feb = jan31 + months{1};
    std::cout << feb << " " << feb.ok() << "\n";

    // Clamp to the last day of the month, or convert to sys_days to overflow.
    year_month_day clamped = feb.ok() ? feb : year_month_day{feb.year() / feb.month() / last};
    std::cout << clamped << " " << year_month_day{sys_days{feb}} << "\n";

    // A calendar day in local time and 24 hours of system time.
    auto ny = locate_zone("America/New_York");
    local_seconds t = local_days{2024y / March / 9} + 18h;
    zoned_time day{ny, t + days{1}};
    zoned_time exact{ny, zoned_time{ny, t}.get_sys_time() + 24h};
    std::cout << std::format("{:%F %R %Z}", day) << "\n";
    std::cout << std::format("{:%F %R %Z}", exact) << "\n";
}
```

```output
2024-02-31 is not a valid date 0
2024-02-29 2024-03-02
2024-03-10 18:00 EDT
2024-03-10 19:00 EDT
```

- Verdict: stdlib. Since C++20, `year_month_day` plus `months` changes only the month, and the result can be invalid, such as February 31. The program chooses: clamp with `last`, or convert to `sys_days` and overflow. A `local_time` plus `days` keeps the wall clock, and a `sys_time` plus `24h` is exact.
