```cpp run id=counts
#include <memory>
#include <print>
#include <string>

struct Order {
    std::string id;
    ~Order() { std::println("destroy {}", id); }
};

int main() {
    std::weak_ptr<Order> probe;
    {
        auto order = std::make_shared<Order>("order 42");
        auto copy = order;
        probe = order;
        std::println("use_count {}", order.use_count());
        if (auto locked = probe.lock()) {
            std::println("locked {}, use_count {}", locked->id, locked.use_count());
        }
        copy.reset();
        std::println("use_count after reset {}", order.use_count());
    }
    std::println("expired {}", probe.expired());
    std::println("lock is empty {}", probe.lock() == nullptr);
}
```

```output
use_count 2
locked order 42, use_count 3
use_count after reset 1
destroy order 42
expired true
lock is empty true
```

- Verdict: stdlib. `std::shared_ptr` counts the `shared_ptr` objects that own one object. The last owner to go destroys the object.
- `std::make_shared` makes the object and the owner in one call. The standard asks it to do no more than one allocation.
- `std::weak_ptr` does not own the object. `lock()` returns an owner while the object exists, and an empty `shared_ptr` after it is gone.
- Copies of a `shared_ptr` in different threads are not a data race, so the library changes the count with atomic operations.
