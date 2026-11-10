```cpp fails expect=sanitizer:address timeout=30 id=vector-overflow
#include <print>
#include <vector>

int main() {
    std::vector<int> scores = {3, 1, 4};
    int total = 0;
    for (std::size_t i = 0; i <= scores.size(); i++) { // one past the end
        total += scores[i];
    }
    std::println("{}", total);
}
```

`operator[]` does not check the index. The vector holds exactly three elements on the heap, so `scores[3]` reads the right redzone of that block, and ASan reports a `heap-buffer-overflow`.

```cpp fails expect=sanitizer:address timeout=30 id=dangling-view
#include <print>
#include <string>
#include <string_view>

std::string greeting(const std::string &name) {
    return "hello, " + name + ", and welcome to the system";
}

int main() {
    std::string_view view = greeting("alice"); // the string dies here
    std::println("{}", view);
}
```

The temporary `std::string` is destroyed at the end of the full expression, and its heap buffer is freed. The view still points to it. ASan reports a `heap-use-after-free`, the error of [[use-after-free]]. The text is longer than the small-string buffer of the library, so the characters are on the heap.

```cpp run flags="-fsanitize=address,undefined" id=checked
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

std::string greeting(const std::string &name) {
    return "hello, " + name + ", and welcome to the system";
}

int main() {
    std::vector<int> scores = {3, 1, 4};
    try {
        std::println("{}", scores.at(3));
    } catch (const std::out_of_range &) {
        std::println("index 3 is out of range");
    }
    std::string text = greeting("alice"); // owns its characters
    std::println("{}", text);
}
```

```output for=checked
index 3 is out of range
hello, alice, and welcome to the system
```

- Verdict: library. ASan is a part of Clang, GCC and MSVC (`/fsanitize=address`), not of the C++ language.
- `at` checks the index and throws `std::out_of_range`. `operator[]`, iterators and raw pointers do not check, so ASan is the check for them in a test build.
- A view type such as `std::string_view` or `std::span` does not own its data. When the owner frees heap data, ASan finds the next use through the view, because the freed block stays poisoned in the quarantine.
- A container library can call into the ASan runtime to mark the unused part of its buffer. ASan then reports a `container-overflow` for an access between `size()` and `capacity()`. The Clang documentation warns that this check can give false positives when only a part of the program is built with ASan.
