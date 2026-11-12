```cpp run id=string-view-clusters-and-truncation
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

// The C++ standard library has no grapheme API. A real program uses ICU
// (icu::BreakIterator::createCharacterInstance). This sketch applies a small
// subset of the UAX 29 rules, with hand-written ranges for this text.

static char32_t decode(std::string_view s, std::size_t i, std::size_t& len) {
    auto b = [&](std::size_t k) { return static_cast<char32_t>(static_cast<unsigned char>(s[i + k])); };
    if (b(0) < 0x80) { len = 1; return b(0); }
    if (b(0) < 0xE0) { len = 2; return ((b(0) & 0x1F) << 6) | (b(1) & 0x3F); }
    if (b(0) < 0xF0) { len = 3; return ((b(0) & 0x0F) << 12) | ((b(1) & 0x3F) << 6) | (b(2) & 0x3F); }
    len = 4;
    return ((b(0) & 0x07) << 18) | ((b(1) & 0x3F) << 12) | ((b(2) & 0x3F) << 6) | (b(3) & 0x3F);
}

static bool is_extend(char32_t c) {
    return (c >= 0x300 && c <= 0x36F) || (c >= 0x1F3FB && c <= 0x1F3FF) || c == 0xFE0F;
}

static bool is_ri(char32_t c) { return c >= 0x1F1E6 && c <= 0x1F1FF; }

static std::vector<std::string_view> graphemes(std::string_view s) {
    std::vector<std::string_view> out;
    std::size_t start = 0;
    char32_t prev = 0;
    int ri_run = 0;
    for (std::size_t i = 0, len = 0; i < s.size(); i += len) {
        char32_t c = decode(s, i, len);
        bool join = i > 0 && (is_extend(c) || c == 0x200D || prev == 0x200D || (is_ri(c) && ri_run % 2 == 1));
        if (!join && i > 0) {
            out.push_back(s.substr(start, i - start));
            start = i;
        }
        ri_run = is_ri(c) ? ri_run + 1 : 0;
        prev = c;
    }
    if (!s.empty()) out.push_back(s.substr(start));
    return out;
}

int main() {
    std::string t = "cafe\u0301 \U0001F1EB\U0001F1F7 \U0001F469\U0001F3FD\u200D\U0001F692";
    auto g = graphemes(t);
    std::printf("%zu bytes, %zu graphemes\n", t.size(), g.size());

    // Truncate to at most 22 bytes. A plain resize cuts the emoji.
    std::string cut = t.substr(0, 22);
    std::size_t keep = 0;
    for (auto x : g) {
        if (keep + x.size() > 22) break;
        keep += x.size();
    }
    std::printf("%zu %02X %zu\n", cut.size(), static_cast<unsigned char>(cut.back()), keep);

    // The last grapheme is one emoji of 15 bytes.
    std::printf("%zu %s\n", g.back().size(), std::string(g.back()).c_str());
}
```

```output for=string-view-clusters-and-truncation
31 bytes, 8 graphemes
22 9F 16
15 👩🏽‍🚒
```

- Verdict: library. The C++ standard library has no grapheme API, so this sketch returns `std::string_view` pieces of the text. A real program uses ICU. `substr(0, 22)` ends with the byte `9F`, inside the skin tone modifier, and a cut at cluster boundaries keeps 16 bytes. The last cluster is the emoji, 15 bytes.
