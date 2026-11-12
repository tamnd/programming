```cpp run id=duration
#include <iostream>
#include <memory>
#include <optional>
#include <string>

class Duration {
public:
    static std::shared_ptr<const Duration> of_seconds(long seconds) {
        static const auto zero = std::shared_ptr<const Duration>(new Duration(0));
        if (seconds == 0) return zero;  // the same instance every time
        return std::shared_ptr<const Duration>(new Duration(seconds));
    }

    static std::shared_ptr<const Duration> of_minutes(long minutes) {
        return of_seconds(minutes * 60);
    }

    static std::optional<std::shared_ptr<const Duration>> parse(const std::string& text) {
        if (text.size() < 2) return std::nullopt;
        char unit = text.back();
        std::string number = text.substr(0, text.size() - 1);
        if ((unit != 's' && unit != 'm') ||
            number.find_first_not_of("0123456789") != std::string::npos) {
            return std::nullopt;  // a constructor cannot return "no object"
        }
        long n = std::stol(number);
        return of_seconds(unit == 'm' ? n * 60 : n);
    }

    std::string str() const { return std::to_string(seconds_) + " s"; }

private:
    explicit Duration(long seconds) : seconds_(seconds) {}
    long seconds_;
};

int main() {
    std::cout << "of_minutes(2) = " << Duration::of_minutes(2)->str() << "\n";
    std::cout << "of_seconds(90) = " << Duration::of_seconds(90)->str() << "\n";
    std::cout << std::boolalpha << "zero is cached: "
              << (Duration::of_seconds(0) == Duration::of_minutes(0)) << "\n";
    auto a = Duration::parse("2m");
    std::cout << "parse(\"2m\") = " << (a ? (*a)->str() : "nullopt") << "\n";
    auto b = Duration::parse("2h");
    std::cout << "parse(\"2h\") = " << (b ? (*b)->str() : "nullopt") << "\n";
}
```

```output
of_minutes(2) = 120 s
of_seconds(90) = 90 s
zero is cached: true
parse("2m") = 120 s
parse("2h") = nullopt
```

- Verdict: idiom, often called the named constructor idiom. The constructor is `private`, and `static` member functions build the object.
- A shared instance needs shared ownership, so the factory returns `std::shared_ptr<const Duration>`. A factory that always makes a new object would return the value or a `std::unique_ptr`.
- `std::make_shared` cannot call a private constructor, so the example calls `new` inside the class.
