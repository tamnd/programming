```cpp run id=seeds
#include <cstddef>
#include <cstdint>
#include <expected>
#include <format>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <vector>

struct Record {
    std::uint8_t tag;
    std::span<const std::uint8_t> value;
};

// Reads records of the form [tag][length][value]; a truncated input is an error value.
std::expected<std::vector<Record>, std::string> parse(std::span<const std::uint8_t> data) {
    std::vector<Record> records;
    std::size_t i = 0;
    while (i < data.size()) {
        if (data.size() - i < 2) {
            return std::unexpected(std::format("truncated header at offset {}", i));
        }
        std::size_t length = data[i + 1];
        if (data.size() - i - 2 < length) {
            return std::unexpected(std::format("truncated value at offset {}", i));
        }
        records.push_back({data[i], data.subspan(i + 2, length)});
        i += 2 + length;
    }
    return records;
}

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    auto result = parse({data, size});
    if (result) {
        std::println("records={}", result->size());
    } else {
        std::println("rejected: {}", result.error());
    }
    return 0;
}

int main() {
    using namespace std::string_view_literals;
    const std::string_view seeds[] = {""sv, "\x01\x02hi"sv, "\x01\x02hi\x02\x00"sv, "z"sv, "~\xe1"sv};
    for (std::string_view seed : seeds) {
        std::vector<std::uint8_t> copy(seed.begin(), seed.end());
        LLVMFuzzerTestOneInput(copy.data(), copy.size());
    }
    std::println("seed corpus: {} inputs passed", std::size(seeds));
}
```

```output
records=0
records=1
records=2
rejected: truncated header at offset 0
rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

`std::vector::at` checks the index and throws `std::out_of_range`. With `at`, the original bug ends the program with an uncaught exception instead of a silent read.

```cpp fails expect=runtime id=no-length-check
#include <cstddef>
#include <cstdint>
#include <print>
#include <string_view>
#include <vector>

// Counts the records of the form [tag][length][value].
std::size_t parse(const std::vector<std::uint8_t>& data) {
    std::size_t count = 0;
    std::size_t i = 0;
    while (i < data.size()) {
        std::size_t length = data.at(i + 1);  // at() checks the index and throws
        count++;
        i += 2 + length;
    }
    return count;
}

int main() {
    using namespace std::string_view_literals;
    const std::string_view seeds[] = {""sv, "\x01\x02hi"sv, "\x01\x02hi\x02\x00"sv, "z"sv, "~\xe1"sv};
    for (std::string_view seed : seeds) {
        std::println("records={}", parse({seed.begin(), seed.end()}));
    }
}
```

- Verdict: library. libFuzzer and AFL fuzz C++ code through the same `extern "C"` entry point as in C.
- `std::span` views the bytes with their size. `operator[]` on a span or a vector does not check the index, so a fuzz build needs AddressSanitizer, as in C.
- `std::expected` returns either the records or an error message, so the parser needs no exceptions. The target prints both cases and never throws.
- `subspan` gives a view of the value with no copy.
