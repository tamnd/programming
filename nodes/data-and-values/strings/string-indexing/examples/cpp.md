```cpp run id=at-and-substr-bytes
#include <cstdio>
#include <stdexcept>
#include <string>

int main() {
    std::string s = "naïve 😀";

    // s[i] is a char, which holds one UTF-8 byte here.
    std::printf("%zu %02X %02X\n", s.size(), static_cast<unsigned char>(s[2]),
                static_cast<unsigned char>(s[3]));

    // at(i) checks the index and throws std::out_of_range.
    try {
        (void)s.at(20);
    } catch (const std::out_of_range&) {
        std::puts("out_of_range");
    }

    // Find the byte where code point 6 starts.
    std::size_t seen = 0, off = std::string::npos;
    for (std::size_t b = 0; b < s.size(); ++b) {
        if ((static_cast<unsigned char>(s[b]) & 0xC0) != 0x80 && seen++ == 6) {
            off = b;
            break;
        }
    }
    std::printf("%zu %s\n", off, s.substr(off).c_str());

    // substr counts bytes, so it can cut a code point in half.
    std::string half = s.substr(0, 3);
    std::printf("%zu %02X\n", half.size(), static_cast<unsigned char>(half.back()));
}
```

```output for=at-and-substr-bytes
11 C3 AF
out_of_range
7 😀
3 C3
```

- Verdict: stdlib. `s[2]` and `s[3]` are the bytes `C3 AF`, and `at(20)` throws `std::out_of_range`. A loop over the bytes finds that code point 6 starts at byte 7. `substr(0, 3)` counts bytes, so it ends with `C3`, the first half of `ï`.
