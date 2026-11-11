```cpp run id=members-and-function-objects
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <utility>

// Composition: CountingSet has-a std::set and shows only what it chooses.
class CountingSet {
public:
    void add(const std::string& x)
    {
        ++added_;
        items_.insert(x);
    }
    template <class It>
    void add_all(It first, It last)
    {
        for (; first != last; ++first)
            add(*first);
    }
    int added() const { return added_; }
    std::size_t size() const { return items_.size(); }

private:
    std::set<std::string> items_;
    int added_ = 0;
};

// The logger holds its sink as a part that callers can replace.
class Logger {
public:
    explicit Logger(std::function<void(const std::string&)> sink) : sink_(std::move(sink)) {}
    void set_sink(std::function<void(const std::string&)> sink) { sink_ = std::move(sink); }
    void log(const std::string& msg) { sink_(msg); }

private:
    std::function<void(const std::string&)> sink_;
};

int main()
{
    CountingSet s;
    s.add("a");
    std::string more[] = {"b", "c", "a"};
    s.add_all(std::begin(more), std::end(more));
    std::cout << s.added() << ' ' << s.size() << '\n';

    Logger log([](const std::string& m) { std::cout << m << '\n'; });
    log.log("ready");
    log.set_sink([](const std::string& m) { std::cout << "[" << m << "]\n"; });
    log.log("ready");
}
```

```output for=members-and-function-objects
4 3
ready
[ready]
```

```cpp wrong id=public-base-exposes-methods
#include <algorithm>
#include <iostream>
#include <vector>

// Inheritance: a sorted vector is-a std::vector, so every vector method comes along.
struct SortedVector : std::vector<int> {
    void add(int x) { insert(std::upper_bound(begin(), end(), x), x); }
    int smallest() const { return front(); }
};

int main()
{
    SortedVector v;
    v.add(5);
    v.add(1);
    v.add(9);
    std::cout << v.smallest() << '\n';
    v.push_back(0); // a public method of the base breaks the order
    v.insert(v.begin(), 7);
    std::cout << v.smallest() << '\n';
}
```

```output for=public-base-exposes-methods
1
7
```

- Verdict: idiom. `CountingSet` holds a `std::set` as a private member and shows only `add`, `add_all`, `added` and `size`. `Logger` holds a `std::function` as its part, and `set_sink` replaces it. A template parameter can also be the part, which the compiler resolves with no run-time cost, as in the allocator parameter of the standard containers.
- The wrong block derives `SortedVector` publicly from `std::vector<int>`. Every public method of the vector is a method of the sorted vector, so `push_back` and `insert` can break the sort order, and `smallest` then returns 7. Hold the vector as a private member, or derive privately and bring back only the safe methods with `using`.
