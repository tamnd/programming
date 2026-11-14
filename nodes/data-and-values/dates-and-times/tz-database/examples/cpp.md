```cpp run id=tzdb
#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>

int main() {
    using namespace std::chrono;
    // get_tzdb() loads the tz database once, from the system or from a copy in the library.
    const tzdb& db = get_tzdb();
    const time_zone* chicago = db.locate_zone("America/Chicago");
    for (sys_days day : {sys_days{1880y / 1 / 1}, sys_days{1936y / 6 / 1}, sys_days{2024y / 7 / 1}}) {
        sys_seconds t = day + 12h;
        sys_info info = chicago->get_info(t);
        std::cout << std::format("{:%Y-%m-%dT%H:%M:%S} {} {}\n", chicago->to_local(t), info.abbrev,
                                 info.offset.count());
    }
    // US/Central is a time_zone_link. locate_zone follows the link to the zone.
    auto link = std::ranges::find(db.links, "US/Central", &time_zone_link::name);
    std::cout << link->name() << " -> " << link->target() << "\n";
    std::cout << db.locate_zone("US/Central")->name() << "\n";
}
```

```output
1880-01-01T06:09:24 LMT -21036
1936-06-01T07:00:00 EST -18000
2024-07-01T07:00:00 CDT -18000
US/Central -> America/Chicago
America/Chicago
```

- Verdict: stdlib. Since C++20, `std::chrono::get_tzdb()` returns the database as a value, with a list of `time_zone` objects and a list of `time_zone_link` objects. `get_info` returns the offset, the abbreviation and the range of the rule for an instant.
