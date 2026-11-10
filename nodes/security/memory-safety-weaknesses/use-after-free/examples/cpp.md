```cpp fails expect=sanitizer:address timeout=30 id=invalidated
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> names{"alice"};
    const std::string& first = names[0];
    names.push_back("bob"); // the vector moves its elements to a new block
    std::println("first: {}", first);
}
```

AddressSanitizer reports a `heap-use-after-free`. There is no `delete` in the program. The vector had room for one element, so `push_back` allocated a new block, moved the elements and freed the old block. The reference `first` still points into the old block.

```cpp run flags="-fsanitize=address,undefined" id=index
#include <cstddef>
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> names{"alice"};
    std::size_t first = 0; // an index stays valid when the vector grows
    names.push_back("bob");
    std::println("first: {}", names[first]);
}
```

```output for=index
first: alice
```

- Verdict: library. The standard says that a reallocation of a vector invalidates all references, pointers and iterators to its elements. Other containers and operations have their own rules, and the compiler does not check them.
- Keep an index or a copy, not a reference, across a call that can change the container.
- `std::unique_ptr` gives each object one owner and frees it once. `std::shared_ptr` keeps an object alive while any owner exists, and `std::weak_ptr` lets code check that the object still exists before it uses it.
- A raw pointer or a reference that outlives its object is the same bug as in C. AddressSanitizer finds it when a test runs the path.
