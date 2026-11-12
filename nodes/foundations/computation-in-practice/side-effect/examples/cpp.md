```cpp run id=const-and-mutable
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Squares {
public:
    long get(long n) const { // const: callers see no change
        ++lookups_; // mutable: a change that callers cannot observe through the value
        auto it = cache_.find(n);
        if (it != cache_.end()) return it->second;
        return cache_[n] = n * n;
    }
    int lookups() const { return lookups_; }

private:
    mutable std::map<long, long> cache_;
    mutable int lookups_ = 0;
};

void append_total(std::vector<int>& xs) { // a non-const reference: xs changes
    int t = 0;
    for (int x : xs) t += x;
    xs.push_back(t);
}

int main() {
    const Squares sq;
    std::cout << sq.get(12) << ' ' << sq.get(12) << ", lookups " << sq.lookups() << '\n';

    std::vector<int> xs{1, 2, 3};
    append_total(xs);
    for (std::size_t i = 0; i < xs.size(); ++i) std::cout << (i ? " " : "") << xs[i];
    std::cout << '\n';
}
```

```output for=const-and-mutable
144 144, lookups 2
1 2 3 6
```

- Verdict: native. A `const` member function promises not to change the object, and a `const` reference parameter promises not to change the argument. A `mutable` member is the exception, for state that callers cannot observe, such as a cache and a counter. A non-`const` reference parameter, as in `append_total`, shows that the function changes the caller's data. Since C++17, the operands of `<<` are evaluated from left to right, so `lookups()` sees both calls of `get`.
