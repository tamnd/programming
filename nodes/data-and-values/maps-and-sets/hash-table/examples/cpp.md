```cpp run id=unordered-map-buckets
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

struct Point {
    int x, y;
    bool operator==(const Point&) const = default;
};

// A hash functor for Point. It mixes the hashes of the fields.
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t h = std::hash<int>{}(p.x);
        return h ^ (std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2));
    }
};

// A legal but bad hash: every key gets the same hash code.
struct ZeroHash {
    std::size_t operator()(int) const noexcept { return 0; }
};

int main() {
    std::unordered_map<Point, std::string, PointHash> grid{{{1, 2}, "found"}};
    std::cout << grid.at({1, 2}) << "\n";

    // The default maximum load factor is 1: on average one element per bucket.
    std::unordered_map<int, int> m;
    std::cout << m.max_load_factor() << "\n";
    for (int i = 0; i < 100; i++) m[i] = i * i;
    std::cout << (m.load_factor() <= m.max_load_factor()) << " " << (m.bucket_count() >= 100) << "\n";

    // Keys with the same hash share a bucket. A hash that returns 0 puts all keys in one.
    std::unordered_map<int, int, ZeroHash> same;
    for (int i = 0; i < 10; i++) same[i] = i;
    std::cout << same.bucket_size(same.bucket(3)) << " " << same.at(7) << "\n";

    // reserve makes room for n elements, so the next n inserts do not rehash.
    std::unordered_map<int, int> sized;
    sized.reserve(1000);
    std::size_t before = sized.bucket_count();
    for (int i = 0; i < 1000; i++) sized[i] = i;
    std::cout << (sized.bucket_count() == before) << "\n";

    // A rehash moves the elements, so it invalidates iterators, but not pointers to elements.
    int* p = &m[7];
    m.rehash(4096);
    std::cout << *p << " " << (m.bucket_count() >= 4096) << "\n";
}
```

```output
found
1
1 1
10 7
1
49 1
```

- Verdict: stdlib. std::unordered_map takes a hash functor for a user type. It exposes its buckets: bucket_count, bucket, bucket_size, load_factor and max_load_factor, which is 1 by default. reserve and rehash set the number of buckets. A rehash keeps pointers to elements valid.
