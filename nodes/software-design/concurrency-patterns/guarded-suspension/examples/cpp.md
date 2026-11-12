```cpp run id=guarded-suspension
#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Tank {
public:
    int lowest = 0, filled = 0, taken = 0, served = 0;  // read after the threads end

    void take(int amount) {
        std::unique_lock<std::mutex> lock(m_);
        // Guarded suspension: wait until the guard is true.
        cv_.wait(lock, [&] { return level_ >= amount; });
        level_ -= amount;
        lowest = std::min(lowest, level_);
        taken += amount;
        served++;
    }

    void fill(int amount) {
        std::lock_guard<std::mutex> lock(m_);
        level_ += amount;
        filled += amount;
        cv_.notify_all();  // each waiting taker checks its guard again
    }

    int level() {
        std::lock_guard<std::mutex> lock(m_);
        return level_;
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    int level_ = 0;
};

int main() {
    Tank tank;
    std::vector<int> amounts = {5, 3, 8};
    std::vector<std::thread> takers;
    for (int a : amounts) takers.emplace_back([&tank, a] { tank.take(a); });
    for (int i = 0; i < 10; i++) tank.fill(2);
    for (auto& t : takers) t.join();

    std::cout << "units filled: " << tank.filled << "\n";
    std::cout << "units taken: " << tank.taken << "\n";
    std::cout << "takers served: " << tank.served << "\n";
    std::cout << "level never below 0: " << (tank.lowest >= 0 ? "True" : "False") << "\n";
    std::cout << "units left: " << tank.level() << "\n";
}
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: stdlib. `std::condition_variable::wait` with a predicate tests the guard after each wakeup.
