```cpp run id=parse-emit deps=deps/cpp/vcpkg.json
#include <nlohmann/json.hpp>
#include <print>
#include <string>
#include <vector>

struct Order {
    std::uint64_t id;
    double total;
    std::vector<std::string> tags;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Order, id, total, tags)

int main() {
    auto text = R"({"id": 9007199254740993, "total": 0.10, "tags": ["new"]})";

    nlohmann::json tree = nlohmann::json::parse(text);
    auto order = tree.get<Order>();
    std::println("{} {} {}", order.id, order.total, order.tags.at(0));

    nlohmann::json back = order;
    std::println("{}", back.dump());
    std::println("{}", back.dump(2));
}
```

```output for=parse-emit
9007199254740993 0.1 new
{"id":9007199254740993,"tags":["new"],"total":0.1}
{
  "id": 9007199254740993,
  "tags": [
    "new"
  ],
  "total": 0.1
}
```

```cpp run id=round-trip deps=deps/cpp/vcpkg.json
#include <cmath>
#include <nlohmann/json.hpp>
#include <print>
#include <string>

int main() {
    using nlohmann::json;

    std::println("{}", json::parse(R"({"total": 1, "total": 2})").dump());
    std::println("{}", json::parse("18446744073709551616").dump());
    std::println("{}", json(NAN).dump());

    auto ordered = nlohmann::ordered_json::parse(R"({"zeta": 1, "alpha": 2})");
    std::println("{}", ordered.dump());

    try {
        json bad = std::string("\xff");
        std::println("{}", bad.dump());
    } catch (const json::type_error& error) {
        std::println("{}", error.what());
    }

    try {
        auto bad = json::parse(R"({"total": 1,})");
        std::println("{}", bad.dump());
    } catch (const json::parse_error& error) {
        std::println("{}", error.what());
    }
}
```

```output for=round-trip
{"total":2}
1.8446744073709552e+19
null
{"zeta":1,"alpha":2}
[json.exception.type_error.316] invalid UTF-8 byte at index 0: 0xFF
[json.exception.parse_error.101] parse error at line 1, column 13: syntax error while parsing object key - unexpected '}'; expected string literal
```

- Verdict: library. The C++ standard library has no JSON. The examples use nlohmann/json 3.12.0, a header-only library. Boost.JSON, RapidJSON and simdjson are other common choices.
- `json::parse` reads text into a `json` value, and `dump()` writes it, compact or with an indent. `get<T>()` and assignment convert to and from a type that has `to_json` and `from_json`, here made by `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE`.
- The `json` type stores objects in a `std::map`, so `dump()` writes the names in sorted order. `nlohmann::ordered_json` keeps the input order.
- An integer keeps 64 bits, as `std::int64_t` or `std::uint64_t`. A larger integer becomes a `double`.
- A repeated name keeps the last value. `NaN` writes as `null`.
- `dump()` throws `type_error.316` for a string that is not valid UTF-8. A syntax error throws `parse_error.101` with the line and the column.
