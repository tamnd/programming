```cpp run id=frames-and-owners
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Node {
    int value;
    std::unique_ptr<Node> next; // the next node is on the heap
};

std::vector<int> make(int n) {
    std::vector<int> v; // the header is in the frame, the elements on the heap
    for (int i = 0; i < n; i++) v.push_back(i);
    return v; // the elements move to the caller, they are not copied
}

int main() {
    std::array<int, 1000> a{}; // 4000 bytes in the frame
    std::vector<int> v = make(1000);
    std::cout << sizeof a << ' ' << sizeof(std::vector<int>) / sizeof(void*) << ' ' << v.size() << '\n';

    auto head = std::make_unique<Node>(Node{1, nullptr});
    head->next = std::make_unique<Node>(Node{2, nullptr});
    std::cout << head->value + head->next->value << '\n';

    std::string small = "hi"; // short strings fit inside the object
    std::string large(100, 'x'); // long strings use a heap buffer
    std::cout << small.size() << ' ' << large.size() << '\n';
}
```

```output for=frames-and-owners
4000 3 1000
3
2 100
```

- Verdict: native. C++ objects have automatic storage in a block, static storage for the run, or dynamic storage from `new`. A `std::array` keeps its elements in the object, so 1000 integers take 4000 bytes of the frame, while a `std::vector` holds three pointers and keeps its elements on the heap. Returning a vector moves its heap buffer to the caller without a copy.
- Smart pointers such as `std::unique_ptr` own heap objects and free them when the owner's scope ends. `std::string` keeps short strings inside the object and moves longer ones to a heap buffer, which is called the small string optimization.
