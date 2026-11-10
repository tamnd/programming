```cpp run id=decode
#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

struct Decoded {
    char32_t code_point;
    std::size_t length;
};

std::optional<Decoded> decode_one(std::string_view bytes) {
    auto byte = [&](std::size_t i) { return static_cast<unsigned char>(bytes[i]); };
    unsigned char lead = byte(0);
    if (lead < 0x80) return Decoded{lead, 1};
    std::size_t length = lead >= 0xC2 && lead <= 0xDF ? 2 : lead >= 0xE0 && lead <= 0xEF ? 3 : lead >= 0xF0 && lead <= 0xF4 ? 4 : 0;
    if (length == 0 || bytes.size() < length) return std::nullopt;
    char32_t value = lead & (0x7F >> length);
    for (std::size_t i = 1; i < length; ++i) {
        if ((byte(i) & 0xC0) != 0x80) return std::nullopt;
        value = value << 6 | (byte(i) & 0x3F);
    }
    static const char32_t minimum[] = {0, 0, 0x80, 0x800, 0x10000};
    if (value < minimum[length] || (value >= 0xD800 && value <= 0xDFFF) || value > 0x10FFFF) return std::nullopt;
    return Decoded{value, length};
}

void show(std::string_view name, std::string_view bytes) {
    std::printf("%-10s %2zu bytes:", std::string(name).c_str(), bytes.size());
    while (!bytes.empty()) {
        auto result = decode_one(bytes);
        if (!result) {
            std::printf(" error at 0x%02X\n", static_cast<unsigned char>(bytes[0]));
            return;
        }
        std::printf(" U+%04X", static_cast<unsigned>(result->code_point));
        bytes.remove_prefix(result->length);
    }
    std::printf("\n");
}

int main() {
    std::u8string text = u8"h\u00E9\u20AC\U0001D11E";
    std::string bytes(text.begin(), text.end());
    std::printf("%zu %zu\n", text.size(), bytes.size());
    show("valid", bytes);
    show("overlong", "\xC0\xAF");
    show("overlong3", "\xE0\x80\xAF");
    show("surrogate", "\xED\xA0\x80");
    show("too big", "\xF4\x90\x80\x80");
    show("truncated", "a\xE2\x82");
}
```

```output for=decode
10 10
valid      10 bytes: U+0068 U+00E9 U+20AC U+1D11E
overlong    2 bytes: error at 0xC0
overlong3   3 bytes: error at 0xE0
surrogate   3 bytes: error at 0xED
too big     4 bytes: error at 0xF4
truncated   3 bytes: U+0061 error at 0xE2
```

- Verdict: idiom. A `std::string` holds bytes and does not know the encoding. The type `char8_t` and the prefix `u8` mark UTF-8 text, and `std::u8string` holds it. The standard library has no function that validates UTF-8 or counts its code points.
- `size` counts bytes. In the example, four characters take 10 bytes.
- The function `decode_one` reads one sequence. It rejects a lead byte that cannot start a sequence, a missing continuation byte, an overlong form, a surrogate and a value above U+10FFFF.
- The facets of `<codecvt>`, such as `std::codecvt_utf8`, are deprecated in the standard ([depr.locale.stdcvt]). Libraries such as ICU and simdutf validate and convert UTF-8, and ICU also does normalization and grapheme segmentation.
