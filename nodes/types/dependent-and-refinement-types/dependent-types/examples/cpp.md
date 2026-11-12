```cpp run id=requires-on-values
#include <array>
#include <cstddef>
#include <iostream>

// A requires-clause is a predicate on the template arguments, checked at
// compile time: first() exists only for arrays that are not empty.
template <typename T, std::size_t N>
    requires(N > 0)
T first(const std::array<T, N>& a) {
    return a[0];
}

// The size of the result is computed from the sizes of the arguments.
template <typename T, std::size_t N, std::size_t M>
std::array<T, N + M> concat(const std::array<T, N>& a, const std::array<T, M>& b) {
    std::array<T, N + M> out{};
    for (std::size_t i = 0; i < N; i++) out[i] = a[i];
    for (std::size_t i = 0; i < M; i++) out[N + i] = b[i];
    return out;
}

int main() {
    std::array<int, 2> a{1, 2};
    std::array<int, 3> b{3, 4, 5};
    auto c = concat(a, b);
    static_assert(c.size() == 5);
    std::cout << "concat size " << c.size() << ", first " << first(c) << "\n";
    std::cout << "first of b: " << first(b) << "\n";
}
```

```output for=requires-on-values
concat size 5, first 1
first of b: 3
```

```cpp fails expect=compile id=empty-array-rejected
#include <array>
#include <cstddef>

template <typename T, std::size_t N>
    requires(N > 0)
T first(const std::array<T, N>& a) {
    return a[0];
}

int main() {
    std::array<int, 0> empty{};
    return first(empty);
}
```

- Verdict: idiom. C++ templates take values as parameters, such as the size `N` of `std::array<T, N>`, and C++20 `requires` clauses state predicates on them that the compiler checks. `concat` computes the size of its result from the sizes of its arguments, `N + M`, as `Vect` does in Idris. The fails block calls `first` on an empty array, and the compiler rejects it because `0 > 0` is false. All these values must be known at compile time.
