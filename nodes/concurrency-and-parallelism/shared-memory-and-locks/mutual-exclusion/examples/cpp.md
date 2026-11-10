```cpp run id=transfer
#include <mutex>
#include <print>
#include <shared_mutex>
#include <thread>
#include <vector>

class Account {
  public:
    explicit Account(int balance) : balance_(balance) {}

    int balance() const {
        std::shared_lock reader(mutex_);
        return balance_;
    }

    friend void transfer(Account& from, Account& to, int amount) {
        std::scoped_lock both(from.mutex_, to.mutex_);
        from.balance_ -= amount;
        to.balance_ += amount;
    }

  private:
    mutable std::shared_mutex mutex_;
    int balance_;
};

int main() {
    Account checking(1000);
    Account savings(1000);
    {
        std::vector<std::jthread> workers;
        for (int i = 0; i < 4; i++) {
            workers.emplace_back([&] {
                for (int k = 0; k < 1000; k++) {
                    transfer(checking, savings, 1);
                    transfer(savings, checking, 1);
                }
            });
        }
    }
    std::println("checking {}, savings {}", checking.balance(), savings.balance());
}
```

```output
checking 1000, savings 1000
```

- Verdict: stdlib. The header `<mutex>` gives `std::mutex` and the guards. `<shared_mutex>` gives the reader-writer lock `std::shared_mutex`.
- `std::scoped_lock` with two mutexes locks both with a deadlock-avoidance algorithm. The two transfers lock the same pair in opposite orders, and the program still finishes.
- `std::shared_lock` takes shared ownership, so many readers of `balance()` can run at once. A write needs exclusive ownership. The mutex is `mutable` so that a `const` method can lock it.
- The `std::jthread` objects join in their destructors at the end of the inner block, before the program prints. A plain `std::lock_guard` is enough when a function takes one mutex.
