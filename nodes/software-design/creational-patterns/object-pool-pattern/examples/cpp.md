```cpp run id=pool
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

struct Buffer {
    explicit Buffer(int id) : id(id) {}  // a real buffer would be costly to create
    int id;
    std::string text;
};

template <typename T>
class Pool {
public:
    using Deleter = std::function<void(T*)>;
    using Handle = std::unique_ptr<T, Deleter>;

    Pool(std::function<std::unique_ptr<T>(int)> create, std::function<void(T&)> reset, std::size_t max_idle)
        : create_(std::move(create)), reset_(std::move(reset)), max_idle_(max_idle) {}

    // The handle returns the item to the pool when it goes out of scope.
    Handle acquire() {
        std::unique_ptr<T> item;
        {
            std::lock_guard<std::mutex> lock(mu_);
            if (!idle_.empty()) {
                item = std::move(idle_.back());
                idle_.pop_back();
            } else {
                item = create_(++created_);
            }
        }
        return Handle(item.release(), [this](T* p) { release(std::unique_ptr<T>(p)); });
    }

    std::size_t idle() {
        std::lock_guard<std::mutex> lock(mu_);
        return idle_.size();
    }
    int created() const { return created_; }

private:
    void release(std::unique_ptr<T> item) {
        reset_(*item);  // clear the state before the next user sees it
        std::lock_guard<std::mutex> lock(mu_);
        if (idle_.size() < max_idle_) idle_.push_back(std::move(item));
        // else: item is destroyed here, the pool is full
    }

    std::function<std::unique_ptr<T>(int)> create_;
    std::function<void(T&)> reset_;
    std::size_t max_idle_;
    std::mutex mu_;
    std::vector<std::unique_ptr<T>> idle_;
    int created_ = 0;
};

int main() {
    Pool<Buffer> pool([](int id) { return std::make_unique<Buffer>(id); },
                      [](Buffer& b) { b.text.clear(); }, 2);
    for (int i = 1; i <= 3; ++i) {
        auto b = pool.acquire();
        b->text = "request " + std::to_string(i);
        std::cout << "request " << i << " -> buffer #" << b->id << ": " << b->text << "\n";
    }  // b goes back to the pool here

    {
        std::vector<Pool<Buffer>::Handle> held;
        for (int i = 0; i < 3; ++i) held.push_back(pool.acquire());
        std::vector<int> ids;
        for (auto& b : held) ids.push_back(b->id);
        std::sort(ids.begin(), ids.end());
        std::cout << "3 at once -> buffers";
        for (int id : ids) std::cout << " #" << id;
        std::cout << "\n";
    }  // all three go back here
    std::cout << "idle after release: " << pool.idle() << "\n";
    std::cout << "created: " << pool.created() << "\n";
}
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: idiom. The standard library has no object pool. A `std::unique_ptr` with a custom deleter returns the item to the pool when the handle is destroyed, so RAII does the release.
- The pool must live longer than every handle, because the deleter holds a pointer to it.
- Pools in C++ often hold memory rather than objects: `std::pmr::unsynchronized_pool_resource` gives pooled allocation to the `std::pmr` containers.
