```cpp run id=stream-read-in-the-condition
#include <print>
#include <sstream>
#include <string>

int main() {
    // The condition reads a word and is false at the end of the stream.
    std::istringstream in("the quick brown fox");
    std::string word;
    int count = 0;
    while (in >> word) {
        ++count;
        std::println("{}: {}", count, word);
    }

    // The same pattern reads numbers.
    std::istringstream nums("4 8 15 16 23 42");
    int x = 0;
    int total = 0;
    while (nums >> x) {
        total += x;
    }
    std::println("total: {}", total);
}
```

```output for=stream-read-in-the-condition
1: the
2: quick
3: brown
4: fox
total: 108
```

- Verdict: native. `in >> word` reads a word and converts the stream to `false` when no word is left, so the read is the condition. The second loop sums numbers with the same pattern.
