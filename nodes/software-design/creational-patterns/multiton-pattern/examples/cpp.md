```cpp run id=multiton
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

class Currency {
public:
    static const Currency& of(const std::string& code) {
        static const std::map<std::string, int> valid{{"USD", 2}, {"EUR", 2}, {"JPY", 0}};
        auto d = valid.find(code);
        if (d == valid.end()) throw std::invalid_argument(code);
        std::lock_guard<std::mutex> lock(mutex_);
        auto& slot = instances_[code];
        if (!slot) slot.reset(new Currency(code, d->second));  // one instance per key
        return *slot;
    }

    static std::size_t count() {
        std::lock_guard<std::mutex> lock(mutex_);
        return instances_.size();
    }

    Currency(const Currency&) = delete;
    Currency& operator=(const Currency&) = delete;

    const std::string code;
    const int decimals;

private:
    Currency(std::string c, int d) : code(std::move(c)), decimals(d) {}
    static inline std::mutex mutex_;
    static inline std::map<std::string, std::unique_ptr<Currency>> instances_;
};

int main() {
    std::cout << "USD has " << Currency::of("USD").decimals << " decimals\n";
    std::cout << "JPY has " << Currency::of("JPY").decimals << " decimals\n";
    std::cout << "same USD object: " << std::boolalpha
              << (&Currency::of("USD") == &Currency::of("USD")) << "\n";
    std::cout << "instances: " << Currency::count() << "\n";
    try {
        Currency::of("XYZ");
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << ": unknown currency\n";
    }

    std::mutex seen_mutex;
    std::set<const Currency*> seen;
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&] {
            const Currency* c = &Currency::of("EUR");
            std::lock_guard<std::mutex> lock(seen_mutex);
            seen.insert(c);
        });
    }
    for (auto& t : threads) t.join();
    std::cout << "8 threads ask for EUR: " << seen.size() << " object\n";
    std::cout << "instances: " << Currency::count() << "\n";
}
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: true
instances: 2
XYZ: unknown currency
8 threads ask for EUR: 1 object
instances: 3
```

- Verdict: idiom. A private constructor, a deleted copy constructor and a static map of `std::unique_ptr` give one object per key. The map owns the objects, and callers get references.
- A `std::mutex` makes the check and the insert one step. A reference into a `std::map` stays valid when other keys are inserted, so callers can keep it.
