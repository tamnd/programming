```cpp run id=lazy
#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Settings {
public:
    const std::map<std::string, int>& config() {
        std::call_once(once_, [this] { config_ = load(); });  // runs on one thread only
        return config_;
    }
    int loads() const { return loads_; }

private:
    std::map<std::string, int> load() {
        std::cout << "loading config\n";  // stands for a slow read of a file
        ++loads_;
        return {{"timeout", 30}};
    }

    std::once_flag once_;
    std::map<std::string, int> config_;
    std::atomic<int> loads_{0};
};

int main() {
    Settings s;
    std::cout << "settings created, loads = " << s.loads() << "\n";
    int first = s.config().at("timeout");  // load before the line is printed
    std::cout << "timeout = " << first << "\n";
    std::cout << "timeout = " << s.config().at("timeout") << "\n";
    std::cout << "loads = " << s.loads() << "\n";

    Settings shared;
    std::atomic<bool> go{false};
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&] {
            while (!go.load()) std::this_thread::yield();  // all eight ask at the same time
            shared.config();
        });
    }
    go = true;
    for (auto& t : threads) t.join();
    std::cout << "8 threads: loads = " << shared.loads() << "\n";
}
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 threads: loads = 1
```

- Verdict: stdlib. `std::call_once` with a `std::once_flag` runs a function once, even when many threads call it together.
- For a single global value, a function-local `static` is simpler. The language initializes it on the first pass through its declaration, and a thread that enters while another initializes it waits.
- `std::optional<T>` holds a lazy value in a single thread with no lock.
