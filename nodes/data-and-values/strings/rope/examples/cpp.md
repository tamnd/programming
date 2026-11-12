```cpp run id=shared-ptr-path-copy
#include <cstdio>
#include <memory>
#include <string>
#include <utility>

// A persistent rope with shared_ptr: an insert builds new nodes on one path and
// shares everything else with the old rope.
struct Node;
using R = std::shared_ptr<const Node>;
struct Node {
    std::string s;
    R l, r;
    std::size_t n;
};

static int made = 0;
static const R kEmpty = std::make_shared<const Node>(Node{"", nullptr, nullptr, 0});

R leaf(std::string s) {
    ++made;
    auto n = s.size();
    return std::make_shared<const Node>(Node{std::move(s), nullptr, nullptr, n});
}

R cat(R a, R b) {
    if (a->n == 0) return b;
    if (b->n == 0) return a;
    ++made;
    auto n = a->n + b->n;
    return std::make_shared<const Node>(Node{"", std::move(a), std::move(b), n});
}

R build(const std::string& s, std::size_t a, std::size_t b) {
    if (b - a <= 16) return leaf(s.substr(a, b - a));
    auto m = (a + b) / 2;
    return cat(build(s, a, m), build(s, m, b));
}

std::pair<R, R> split(const R& t, std::size_t i) {
    if (i == 0) return {kEmpty, t};
    if (i >= t->n) return {t, kEmpty};
    if (!t->l) return {leaf(t->s.substr(0, i)), leaf(t->s.substr(i))};
    if (i < t->l->n) {
        auto [a, b] = split(t->l, i);
        return {a, cat(b, t->r)};
    }
    auto [a, b] = split(t->r, i - t->l->n);
    return {cat(t->l, a), b};
}

R insert(const R& t, std::size_t i, std::string s) {
    auto [a, b] = split(t, i);
    return cat(cat(a, leaf(std::move(s))), b);
}

void text(const R& t, std::string& out) {
    if (!t->l) {
        out += t->s;
        return;
    }
    text(t->l, out);
    text(t->r, out);
}

int main() {
    std::string big;
    for (int k = 0; k < 4096; ++k) big += static_cast<char>('a' + k % 26);
    R r = build(big, 0, big.size());
    std::printf("%zu %d\n", r->n, made);

    // The insert makes a few nodes. std::string::insert moves every byte after the position.
    made = 0;
    R r2 = insert(r, 2050, "XYZ");
    std::string flat = big;
    flat.insert(2050, "XYZ");
    std::printf("%d %zu\n", made, big.size() - 2050);

    std::string out;
    text(r2, out);
    std::printf("%zu %s %d\n", r2->n, out.substr(2047, 9).c_str(), out == flat);
}
```

```output for=shared-ptr-path-copy
4096 511
13 2046
4099 tuvXYZwxy 1
```

- Verdict: idiom. Standard C++ has no rope. The SGI `rope` class is an extension. The rope of 4096 characters has 256 leaves and 511 nodes. An insert at position 2050 makes 13 nodes: the two halves of the cut leaf, the leaf for the new text and the join nodes on the path. `std::string::insert` moves the 2046 bytes after the position.
