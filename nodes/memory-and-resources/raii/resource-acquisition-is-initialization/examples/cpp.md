```cpp run id=destructors
#include <memory>
#include <mutex>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>

class Handle {
public:
    explicit Handle(std::string name) : name_(std::move(name)) {
        std::println("acquire {}", name_);
    }
    ~Handle() { std::println("release {}", name_); }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

private:
    std::string name_;
};

std::mutex orders_mutex;
int order_count = 0;

void add_order(bool fail) {
    std::lock_guard<std::mutex> guard(orders_mutex);
    Handle log("log");
    Handle cache("cache");
    if (fail) {
        throw std::runtime_error("bad order");
    }
    ++order_count;
}

int main() {
    add_order(false);
    try {
        add_order(true);
    } catch (const std::exception& error) {
        std::println("caught {}", error.what());
    }
    bool free_after_throw = orders_mutex.try_lock();
    std::println("mutex free after throw: {}", free_after_throw);
    if (free_after_throw) {
        orders_mutex.unlock();
    }
    auto buffer = std::make_unique<Handle>("buffer");
    std::println("orders: {}", order_count);
}
```

```output
acquire log
acquire cache
release cache
release log
acquire log
acquire cache
release cache
release log
caught bad order
mutex free after throw: true
acquire buffer
orders: 1
release buffer
```

- Verdict: native. The destructor of each local object runs when its block exits, in the reverse order of construction. Here `cache` is released before `log`.
- `std::lock_guard` locks the mutex in its constructor and unlocks it in its destructor. After the exception, `try_lock` succeeds, so stack unwinding unlocked the mutex.
- `std::make_unique` puts the buffer on the heap. The `std::unique_ptr` deletes it when `main` returns, after the last line of output.
- `Handle` deletes its copy operations. A copy would release the same resource twice.
