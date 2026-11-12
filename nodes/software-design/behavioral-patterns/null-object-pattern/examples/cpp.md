```cpp run id=null-logger
#include <iostream>
#include <string>

struct Logger {
    virtual ~Logger() = default;
    virtual void log(const std::string& message) = 0;
};

struct ConsoleLogger : Logger {
    void log(const std::string& message) override {
        std::cout << "log: " << message << "\n";
    }
};

struct NullLogger : Logger {
    void log(const std::string&) override {}  // do nothing
};

class Checkout {
public:
    explicit Checkout(Logger& logger) : logger_(logger) {}

    std::string pay(int amount) {
        logger_.log("paid " + std::to_string(amount));
        return "receipt: " + std::to_string(amount);
    }

private:
    Logger& logger_;
};

int main() {
    ConsoleLogger console;
    NullLogger silent;
    std::cout << Checkout(console).pay(10) << "\n";
    std::cout << Checkout(silent).pay(20) << "\n";
}
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. `NullLogger` overrides the pure virtual function with an empty body. The checkout holds a reference to a `Logger`.
