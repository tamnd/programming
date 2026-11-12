```cpp run id=guard
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::mutex lock;
std::map<std::string, int> table;

int increment(const std::string& path) {
    std::lock_guard<std::mutex> guard(lock);  // the destructor releases the lock
    if (path.empty()) return -1;
    ++table[path];
    return 0;
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([] { for (int j = 0; j < 1000; ++j) increment("/index"); });
    }
    for (auto& t : threads) t.join();
    std::cout << "hits /index: " << table["/index"] << "\n";
    std::cout << "bad path returns " << increment("") << "\n";
    bool free = lock.try_lock();
    if (free) lock.unlock();
    std::cout << "lock free after the early return: " << (free ? "True" : "False") << "\n";
    increment("/index");
    std::cout << "hits /index: " << table["/index"] << "\n";
}
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: native. `std::lock_guard` is the guard class of the idiom, and its destructor runs on each return path.
