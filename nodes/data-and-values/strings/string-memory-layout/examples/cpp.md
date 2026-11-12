```cpp run id=small-string-optimization
#include <iostream>
#include <string>
#include <string_view>

// is_inline reports whether the characters are inside the string object,
// which is the small string optimization.
bool is_inline(const std::string& s) {
    const char* object = reinterpret_cast<const char*>(&s);
    return s.data() >= object && s.data() < object + sizeof s;
}

int main() {
    std::string small = "hi";
    std::string large(100, 'x');
    std::cout << std::boolalpha;
    std::cout << "small inline: " << is_inline(small) << '\n';
    std::cout << "large inline: " << is_inline(large) << '\n';

    // An empty string already has room for some characters without allocation.
    std::string empty;
    std::cout << "empty capacity >= 15: " << (empty.capacity() >= 15) << '\n';

    // A std::string_view is a pointer and a length into other memory.
    std::string_view view(large.data() + 10, 5);
    std::cout << view << ' ' << (view.data() == large.data() + 10) << '\n';

    // The characters are contiguous and a null character follows them.
    std::cout << (large.data()[large.size()] == '\0') << '\n';
}
```

```output for=small-string-optimization
small inline: true
large inline: false
empty capacity >= 15: true
xxxxx true
true
```

- Verdict: stdlib. The characters of the short string `"hi"` are inside the `std::string` object, which is the small string optimization. The 100 characters of `large` are in a heap buffer. An empty string has room for at least 15 characters with no allocation. A `std::string_view` points into the buffer of `large`, and a null character follows the last character.
