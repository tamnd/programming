```cpp fails expect=sanitizer:address timeout=30 id=index
#include <cstddef>
#include <print>
#include <vector>

void set_score(std::vector<int>& scores, std::size_t player, int score) {
    scores[player] = score; // operator[] does not check the index
}

int main() {
    std::vector<int> scores(4);
    set_score(scores, 2, 10);
    std::println("player 2: {}", scores[2]);
    set_score(scores, 4, 99);
}
```

AddressSanitizer reports a `heap-buffer-overflow` and a `WRITE of size 4` in `set_score`. The vector has four elements, and index 4 is one past the end.

```cpp run flags="-fsanitize=address,undefined" id=at
#include <cstddef>
#include <print>
#include <stdexcept>
#include <vector>

void set_score(std::vector<int>& scores, std::size_t player, int score) {
    scores.at(player) = score; // at() checks the index and throws
}

int main() {
    std::vector<int> scores(4);
    set_score(scores, 2, 10);
    std::println("player 2: {}", scores[2]);
    try {
        set_score(scores, 4, 99);
    } catch (const std::out_of_range&) {
        std::println("refused: player 4 is out of range, the size is {}", scores.size());
    }
}
```

```output for=at
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: library. The containers carry their size, and `at` throws `std::out_of_range` when the index is not less than `size()`. `operator[]`, iterators and raw pointers do not check.
- The working draft of the C++ standard lists `n < a.size()` as a hardened precondition of `a[n]`. A hardened implementation checks it and ends the program when it fails. In an implementation that is not hardened, a violation is undefined behavior.
- Prefer `std::vector`, `std::string`, `std::array` and `std::span` to raw arrays, and range-based `for` loops to index arithmetic. Each one keeps the size next to the data.
- AddressSanitizer finds the bug only when a test runs the line with a bad index. Fuzzing gives the tests many inputs.
