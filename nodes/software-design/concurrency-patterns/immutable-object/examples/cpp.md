```cpp run id=immutable-object
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct Settings { // const members: set in the constructor, never changed
    const int port;
    const std::string tag;
    Settings(int p, std::string t) : port(p), tag(std::move(t)) {}
};

std::mutex pointer_mutex; // guards only the pointer, not the object
std::shared_ptr<const Settings> current;

std::shared_ptr<const Settings> load() {
    std::lock_guard<std::mutex> lock(pointer_mutex);
    return current;
}

void publish(std::shared_ptr<const Settings> s) {
    std::lock_guard<std::mutex> lock(pointer_mutex);
    current = std::move(s);
}

int main() {
    auto first = std::make_shared<const Settings>(8000, "port 8000");
    publish(first);
    std::atomic<int> reads{0}, mixed{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([&] {
            for (int r = 0; r < 1000; r++) {
                auto s = load(); // the object itself is read with no lock
                std::this_thread::yield();
                if (s->tag != "port " + std::to_string(s->port)) mixed++;
                reads++;
            }
        });
    }
    threads.emplace_back([] {
        for (int port = 8001; port <= 8100; port++) {
            publish(std::make_shared<const Settings>(port, "port " + std::to_string(port)));
            std::this_thread::yield();
        }
    });
    for (auto& t : threads) t.join();
    bool unchanged = first->port == 8000 && first->tag == "port 8000";
    std::cout << "reads: " << reads << "\n";
    std::cout << "reads that saw a mixed pair: " << mixed << "\n";
    std::cout << "settings published: " << 1 + load()->port - first->port << "\n";
    std::cout << "first settings unchanged: " << (unchanged ? "True" : "False") << "\n";
}
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: idiom. `Settings` has `const` members, the threads share it through `std::shared_ptr<const Settings>`, and a mutex guards only the copy of the pointer.
