```cpp run id=settings
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

class Settings {
public:
    static Settings& instance() {
        // Initialized the first time control passes here; concurrent callers wait.
        static Settings settings;
        return settings;
    }

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    std::string level() const {
        std::lock_guard lock(mutex_);
        return level_;
    }

    void set_level(std::string level) {
        std::lock_guard lock(mutex_);
        level_ = std::move(level);
    }

private:
    Settings() { std::cout << "creating the settings\n"; }

    mutable std::mutex mutex_;
    std::string level_ = "info";
};

int main() {
    Settings& first = Settings::instance();
    std::cout << "main: " << first.level() << "\n";
    const Settings* seen = nullptr;
    std::thread worker([&seen] {
        Settings& settings = Settings::instance();
        std::cout << "worker sets debug\n";
        settings.set_level("debug");
        seen = &settings;
    });
    worker.join();
    std::cout << "main: " << Settings::instance().level() << "\n";
    std::cout << "same instance: " << std::boolalpha << (seen == &first) << "\n";
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: idiom. A function-local static is the usual C++ singleton. The standard makes its initialization run once, and makes concurrent callers wait for it.
- The deleted copy constructor and copy assignment stop code from making a copy of the instance.
- A global object at namespace scope would be the eager form, but its dynamic initialization is not ordered with globals in other translation units. The function-local static avoids that order problem.
- The object is destroyed at exit, in reverse order of construction. A singleton that uses another singleton in its destructor can see an object that is already destroyed.
