```cpp run id=transform-and-ascii-compare
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>

int main() {
    // std::toupper takes one char, so a std::transform maps byte by byte.
    // The cast to unsigned char avoids undefined behaviour for bytes over 0x7F.
    std::string s = "straße";
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    std::printf("%s %zu\n", s.c_str(), s.size());

    // A caseless compare of ASCII letters with a predicate.
    auto same = [](unsigned char a, unsigned char b) { return std::tolower(a) == std::tolower(b); };
    std::string a = "Content-Type", b = "content-type";
    std::printf("%d\n", a.size() == b.size() && std::ranges::equal(a, b, same));

    // The bytes of "ß" are not letters in the "C" locale, so they do not change.
    std::string u8 = "ß";
    std::printf("%02X %02X %d\n", static_cast<unsigned char>(u8[0]), static_cast<unsigned char>(u8[1]),
                std::isalpha(static_cast<unsigned char>(u8[0])) != 0);
}
```

```output for=transform-and-ascii-compare
STRAßE 7
1
C3 9F 0
```

- Verdict: stdlib. `std::toupper` takes one `char`, so `std::ranges::transform` maps byte by byte and `ß` keeps its 2 bytes. A predicate with `std::tolower` finds `Content-Type` and `content-type` equal. The bytes `C3 9F` of `ß` are not letters in the C locale, so `isalpha` gives 0.
