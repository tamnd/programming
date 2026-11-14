```cpp run id=zoned-time
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    const time_zone* ny = locate_zone("America/New_York");
    for (sys_days d : {sys_days{2024y / January / 9}, sys_days{2024y / July / 9}}) {
        zoned_time z{ny, d + 12h};
        std::cout << std::format("{:%FT%H:%M%z %Z}", z) << '\n';
    }

    // get_info tells whether a local time is in a gap or an overlap.
    local_seconds gap = local_days{2024y / March / 10} + 2h + 30min;
    std::cout << (ny->get_info(gap).result == local_info::nonexistent) << '\n';
    local_seconds over = local_days{2024y / November / 3} + 1h + 30min;
    local_info oi = ny->get_info(over);
    std::cout << (oi.result == local_info::ambiguous) << ' ' << oi.first.offset << ' '
              << oi.second.offset << '\n';

    // choose::latest picks the second of the two instants.
    zoned_time later{ny, over, choose::latest};
    std::cout << std::format("{:%FT%H:%M%z}", later) << '\n';
}
```

```output
2024-01-09T07:00-0500 EST
2024-07-09T08:00-0400 EDT
1
1 -14400s -18000s
2024-11-03T01:30-0500
```

- Verdict: stdlib. std::chrono has time_zone from the IANA database through locate_zone, and zoned_time pairs a zone with a time point. get_info finds gaps and overlaps, and choose::earliest and choose::latest resolve an overlap.
