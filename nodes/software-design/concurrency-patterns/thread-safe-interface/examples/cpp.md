```cpp run id=interface
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

class FileCache {
public:
    // Interface methods: take the lock, then call an implementation method.
    std::string find(const std::string& path) {
        std::lock_guard<std::mutex> guard(lock_);
        ++acquires;
        return find_i(path);
    }

    void bind(const std::string& path) {
        std::lock_guard<std::mutex> guard(lock_);
        ++acquires;
        bind_i(path);
    }

    std::map<std::string, std::string> files;
    int acquires = 0;

private:
    // Implementation methods: trust that the lock is held, never take it.
    std::string find_i(const std::string& path) {
        auto file = check_cache_i(path);
        if (!file) {
            bind_i(path);  // not bind(): that would take the lock again
            file = check_cache_i(path);
        }
        return *file;
    }

    std::optional<std::string> check_cache_i(const std::string& path) {
        auto it = files.find(path);
        if (it == files.end()) return std::nullopt;
        return it->second;
    }

    void bind_i(const std::string& path) { files[path] = "mapped " + path; }

    std::mutex lock_;  // not recursive
};

int main() {
    FileCache cache;
    cache.bind("/index.html");
    const char* paths[] = {"/a.html", "/b.html", "/c.html"};
    std::vector<std::thread> threads;
    for (int w = 0; w < 4; ++w)
        threads.emplace_back([&] {
            for (int i = 0; i < 1000; ++i) cache.find(paths[i % 3]);
        });
    for (auto& t : threads) t.join();
    std::cout << "find /a.html: " << cache.find("/a.html") << "\n";
    std::cout << "files cached: " << cache.files.size() << "\n";
    std::cout << "lock acquisitions: " << cache.acquires << "\n";
}
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. Public members are the interface methods and private members are the implementation methods, which is the split the paper uses for C++.
