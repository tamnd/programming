```cpp run id=scoped
#include <iostream>
#include <mutex>
#include <thread>

struct Account {
    int balance;
    std::mutex m;
};

void transfer(Account& from, Account& to, int amount) {
    std::scoped_lock lock(from.m, to.m);
    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account a{1000, {}};
    Account b{1000, {}};
    std::thread t1([&] {
        for (int i = 0; i < 10000; ++i) transfer(a, b, 1);
    });
    std::thread t2([&] {
        for (int i = 0; i < 10000; ++i) transfer(b, a, 2);
    });
    t1.join();
    t2.join();
    std::cout << "a: " << a.balance << ", b: " << b.balance
              << ", total: " << a.balance + b.balance << '\n';
}
```

```output for=scoped
a: 11000, b: -9000, total: 2000
```

- Verdict: stdlib. `std::mutex`, `std::timed_mutex` and `std::recursive_mutex` are in `<mutex>`. `std::lock` and `std::scoped_lock` lock several mutexes without deadlock.
- The two threads name the mutexes in opposite orders. Two `std::lock_guard` objects in that order could deadlock. One `std::scoped_lock` with both mutexes does not, because it calls `std::lock`.
- `std::timed_mutex::try_lock_for` gives a lock with a timeout for the back off method.
- A second `lock` of a `std::mutex` by the thread that owns it is undefined behaviour. `std::recursive_mutex` allows it.
