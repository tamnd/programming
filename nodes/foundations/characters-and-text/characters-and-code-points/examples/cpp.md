```cpp run id=char8-char16-char32
#include <iostream>
#include <string>

int main() {
    std::u8string s8 = u8"cafe\u0301 \U0001F44D\U0001F3FD"; // UTF-8 code units
    std::u16string s16 = u"cafe\u0301 \U0001F44D\U0001F3FD"; // UTF-16 code units
    std::u32string s32 = U"cafe\u0301 \U0001F44D\U0001F3FD"; // one unit per code point
    std::cout << s8.size() << ' ' << s16.size() << ' ' << s32.size() << '\n';

    std::cout << std::hex << std::uppercase << static_cast<unsigned>(s16[6]) << ' '
              << static_cast<unsigned>(s32[6]) << std::dec << '\n';

    std::string bytes = "caf\xC3\xA9"; // std::string holds bytes, whatever they encode
    std::cout << bytes.size() << ' ' << sizeof(char8_t) << ' ' << sizeof(char16_t) << ' '
              << sizeof(char32_t) << '\n';
}
```

```output for=char8-char16-char32
15 10 8
D83D 1F44D
5 1 2 4
```

- Verdict: native. C++ has string types for each Unicode encoding form: `std::u8string` of `char8_t` for UTF-8, `std::u16string` for UTF-16 and `std::u32string` for UTF-32, and the literal prefixes `u8`, `u` and `U` encode a literal in each form. The sample takes 15, 10 and 8 units in the three forms. `std::string` holds bytes in any encoding.
- The standard library has no normalization, segmentation or case mapping for Unicode. Programs use ICU or a similar library for them.
