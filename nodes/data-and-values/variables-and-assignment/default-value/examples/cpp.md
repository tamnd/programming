```cpp run id=member-defaults
#include <iostream>
#include <map>
#include <string>

struct Options {
    int width = 80;
    bool verbose{};
    std::string name;
};

int main() {
    Options o;
    std::cout << o.width << ' ' << o.verbose << " [" << o.name << "]\n";
    std::map<std::string, int> counts;
    counts["a"] += 1;
    std::cout << counts["a"] << ' ' << counts["b"] << ' ' << counts.size() << '\n';
    std::cout << int{} << ' ' << double{} << '\n';
}
```

```output for=member-defaults
80 0 []
1 0 2
0 0
```

- Verdict: native. A default member initializer such as `int width = 80;` gives a field its value in each new object. `{}` gives the zero value of a built-in type. `std::map::operator[]` inserts a value-initialized element for a missing key, so the read of `counts["b"]` adds a key.
