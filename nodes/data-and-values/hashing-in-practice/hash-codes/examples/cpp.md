```cpp run id=std-hash-specialization
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

struct Point {
    int x, y;
    bool operator==(const Point&) const = default;
};

// The boost::hash_combine idiom: mix each field into the seed
inline void hash_combine(std::size_t& seed, std::size_t h) {
    seed ^= h + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

template <>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t seed = 0;
        hash_combine(seed, std::hash<int>{}(p.x));
        hash_combine(seed, std::hash<int>{}(p.y));
        return seed;
    }
};

int main() {
    std::unordered_map<Point, std::string> points{{{1, 2}, "a"}};
    std::cout << "struct key: " << points.at({1, 2}) << "\n";
    std::cout << "swapped fields differ: " << std::boolalpha
              << (std::hash<Point>{}({1, 2}) != std::hash<Point>{}({2, 1})) << "\n";

    char buf[] = "ada";
    const char* lit = "ada";
    std::cout << "hash<const char*> equal: "
              << (std::hash<const char*>{}(buf) == std::hash<const char*>{}(lit)) << "\n";
    std::cout << "hash<string_view> equal: "
              << (std::hash<std::string_view>{}(buf) == std::hash<std::string_view>{}(lit)) << "\n";
}
```

```output for=std-hash-specialization
struct key: a
swapped fields differ: true
hash<const char*> equal: false
hash<string_view> equal: true
```

```cpp fails expect=compile id=no-hash-for-point
#include <unordered_set>

struct Point {
    int x, y;
    bool operator==(const Point&) const = default;
};

int main() {
    std::unordered_set<Point> seen;
    seen.insert({1, 2});
    return static_cast<int>(seen.size());
}
```

- Verdict: stdlib. `std::hash` and the unordered containers are in the standard library. A type gets a hash from a specialization of `std::hash`, and the hash_combine idiom from Boost mixes the hashes of the fields. `std::hash<const char*>` hashes the pointer, not the characters, so two equal strings at different addresses hash differently; hash a `std::string_view` instead. The fails block puts a type with no `std::hash` into an `unordered_set`, and the compiler rejects it.
