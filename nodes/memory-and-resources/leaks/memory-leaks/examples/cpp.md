```cpp fails expect=sanitizer:leak timeout=30 id=raw-new-throws
#include <print>
#include <stdexcept>
#include <string>

struct Order {
    std::string id;
    int quantity;
};

int checked_quantity(const Order& order) {
    if (order.quantity <= 0) {
        throw std::invalid_argument("bad quantity in " + order.id);
    }
    return order.quantity;
}

int process(int quantity) {
    Order* order = new Order{"A-1", quantity};
    int result = checked_quantity(*order); // a throw skips the delete below
    delete order;
    return result;
}

int main() {
    std::println("processed: {}", process(3));
    try {
        process(0);
    } catch (const std::invalid_argument& e) {
        std::println("error: {}", e.what());
    }
}
```

The program prints the error and exits normally. LeakSanitizer then reports the `Order` from the second call. Stack unwinding destroys local objects, but `order` is a raw pointer, and destroying a pointer does not delete its target.

```cpp run id=unique-ptr
#include <memory>
#include <print>
#include <stdexcept>
#include <string>

struct Order {
    std::string id;
    int quantity;
    ~Order() { std::println("order {} deleted", id); }
};

int checked_quantity(const Order& order) {
    if (order.quantity <= 0) {
        throw std::invalid_argument("bad quantity in " + order.id);
    }
    return order.quantity;
}

int process(int quantity) {
    auto order = std::make_unique<Order>("A-1", quantity);
    return checked_quantity(*order); // unwinding destroys order, which deletes the Order
}

int main() {
    std::println("processed: {}", process(3));
    try {
        process(0);
    } catch (const std::invalid_argument& e) {
        std::println("error: {}", e.what());
    }
}
```

```output
order A-1 deleted
processed: 3
order A-1 deleted
error: bad quantity in A-1
```

- Verdict: stdlib. `std::unique_ptr` owns the object. Its destructor runs on a return and on stack unwinding, so each path deletes the `Order`.
- The output shows `order A-1 deleted` before `error:`. The destructor runs while the exception leaves `process`, before the handler in `main` starts.
- A raw `new` with a later `delete` leaks on each path that skips the `delete`: an early `return`, a `throw`, or a `break`.
