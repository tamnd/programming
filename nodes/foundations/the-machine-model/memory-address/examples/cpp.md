```cpp run id=vector-moves-its-elements
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30};
    auto before = reinterpret_cast<std::uintptr_t>(v.data()); // keep the address as a number
    v.reserve(1000); // the vector moves its elements to a new block
    auto after = reinterpret_cast<std::uintptr_t>(v.data());
    std::cout << "same block after reserve: " << std::boolalpha << (before == after) << '\n';
    // A pointer kept from before the move would point to freed memory.

    int x = 7;
    int& r = x;     // a reference is another name for the object at an address
    int* p = std::addressof(x);
    std::cout << "&r == p: " << (&r == p) << '\n';

    auto n = reinterpret_cast<std::uintptr_t>(p); // the address as an integer
    std::cout << "aligned to alignof(int): " << (n % alignof(int) == 0) << '\n';

    struct Node { char c; double d; };
    std::cout << "offsetof(Node, d) = " << offsetof(Node, d) << ", sizeof(Node) = " << sizeof(Node) << '\n';
}
```

```output for=vector-moves-its-elements
same block after reserve: false
&r == p: true
aligned to alignof(int): true
offsetof(Node, d) = 8, sizeof(Node) = 16
```

- Verdict: native. C++ has the pointers of C and adds references, which are other names for an object, so `&r` is the address of `x`. `std::addressof` gives the address even for a type that overloads `&`. A `std::vector` keeps its elements in one block and moves them to a new block when it grows, so `reserve` changes `data()`, and a pointer kept from before would point to freed memory. The `double` in `Node` is at offset 8, after 7 bytes of padding.
