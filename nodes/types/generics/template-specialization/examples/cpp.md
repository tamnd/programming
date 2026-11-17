```cpp run id=full-and-partial
#include <iostream>
#include <vector>

// The primary template.
template <typename T, typename U>
struct Pair {
    static const char* kind() { return "two types"; }
};

// A partial specialization: both types are the same.
template <typename T>
struct Pair<T, T> {
    static const char* kind() { return "one type twice"; }
};

// A partial specialization: the second type is a pointer.
template <typename T, typename U>
struct Pair<T, U*> {
    static const char* kind() { return "second is a pointer"; }
};

// A full specialization.
template <>
struct Pair<int, double> {
    static const char* kind() { return "int and double"; }
};

int main() {
    std::cout << Pair<char, long>::kind() << "\n";
    std::cout << Pair<long, long>::kind() << "\n";
    std::cout << Pair<char, int*>::kind() << "\n";
    std::cout << Pair<int, double>::kind() << "\n";

    std::vector<bool> bits{true, false};
    auto ref = bits[0];  // a proxy object, not a bool&
    ref = false;
    std::cout << std::boolalpha << bits[0] << "\n";
}
```

```output for=full-and-partial
two types
one type twice
second is a pointer
int and double
false
```

- Verdict: native. `Pair<T, T>` and `Pair<T, U*>` are partial specializations, and `Pair<int, double>` is a full specialization. The compiler picks the most specialized version that matches. `bits[0]` gives a `std::vector<bool>::reference`, so `auto` makes a proxy, and writing to it changes the vector.
