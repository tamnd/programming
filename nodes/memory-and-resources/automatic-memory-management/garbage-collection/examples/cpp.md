```cpp run id=cycle
#include <memory>
#include <print>

struct Item {
    std::shared_ptr<Item> strong_peer;
    std::weak_ptr<Item> weak_peer;
};

// Make a pair a -> b -> a and return a weak probe to a.
std::weak_ptr<Item> pair(bool back_link_is_weak) {
    auto a = std::make_shared<Item>();
    auto b = std::make_shared<Item>();
    a->strong_peer = b;
    if (back_link_is_weak) {
        b->weak_peer = a;
    } else {
        b->strong_peer = a;
    }
    return a;
}

int main() {
    std::weak_ptr<Item> leaked = pair(false);
    std::println("strong cycle freed: {}", leaked.expired());
    std::println("weak back link freed: {}", pair(true).expired());
    // Break the cycle by hand, or LeakSanitizer reports it at exit.
    if (auto a = leaked.lock()) {
        a->strong_peer.reset();
    }
    std::println("freed after manual break: {}", leaked.expired());
}
```

```output
strong cycle freed: false
weak back link freed: true
freed after manual break: true
```

- Verdict: stdlib. C++ has no collector. `std::shared_ptr` counts the owners of an object and destroys it when the last owner goes away.
- A cycle of `shared_ptr` values never reaches a count of zero, so it leaks. `std::weak_ptr` observes an object without owning it, and `expired()` tells whether the object is gone.
- C++11 added an interface for garbage collectors, but C++23 removed it.
