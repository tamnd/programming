```cpp run id=strategy
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// A lock for one thread: both functions do nothing.
struct NullMutex {
    void lock() {}
    void unlock() {}
};

template <class LOCK>  // the locking strategy is a parameter
class FileCache {
public:
    std::string find(const std::string& path) {
        std::lock_guard<LOCK> guard(lock_);
        ++lookups;
        auto it = files.find(path);
        if (it == files.end()) it = files.emplace(path, "mapped " + path).first;
        return it->second;
    }
    std::map<std::string, std::string> files;
    int lookups = 0;

private:
    LOCK lock_;
};

int main() {
    FileCache<NullMutex> single;
    for (const char* path : {"/a.html", "/b.html", "/a.html"}) single.find(path);
    std::cout << "Null_Mutex cache: " << single.files.size() << " files, "
              << single.lookups << " lookups\n";

    FileCache<std::mutex> multi;
    const char* paths[] = {"/a.html", "/b.html", "/c.html"};
    std::vector<std::thread> threads;
    for (int w = 0; w < 4; ++w)
        threads.emplace_back([&] {
            for (int i = 0; i < 1000; ++i) multi.find(paths[i % 3]);
        });
    for (auto& t : threads) t.join();
    std::cout << "Thread_Mutex cache: " << multi.files.size() << " files, "
              << multi.lookups << " lookups\n";
}
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. This is the parameterized type form. `std::lock_guard<LOCK>` accepts `NullMutex` because it has `lock` and `unlock`.
