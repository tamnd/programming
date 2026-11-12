```cpp run id=wrappers
#include <iostream>
#include <optional>

struct Thermometer {
    virtual ~Thermometer() = default;
    virtual int read() = 0;
};

struct Sensor : Thermometer {
    int reads = 0;
    int read() override {
        ++reads;
        return 20;
    }
};

class FahrenheitAdapter {
public:
    explicit FahrenheitAdapter(Thermometer& sensor) : sensor_(sensor) {}
    int fahrenheit() { return sensor_.read() * 9 / 5 + 32; }

private:
    Thermometer& sensor_;
};

class Calibrated : public Thermometer {
public:
    Calibrated(Thermometer& sensor, int offset) : sensor_(sensor), offset_(offset) {}
    int read() override { return sensor_.read() + offset_; }

private:
    Thermometer& sensor_;
    int offset_;
};

class CachingProxy : public Thermometer {
public:
    explicit CachingProxy(Thermometer& sensor) : sensor_(sensor) {}
    int read() override {
        if (!value_) value_ = sensor_.read();
        return *value_;
    }

private:
    Thermometer& sensor_;
    std::optional<int> value_;
};

int main() {
    Sensor a, b, real;
    std::cout << "adapter: " << FahrenheitAdapter(a).fahrenheit() << " F\n";
    std::cout << "decorator: " << Calibrated(b, 1).read() << "\n";
    CachingProxy proxy(real);
    int first = proxy.read();
    int second = proxy.read();
    std::cout << "proxy: " << first << " " << second << ", real reads: " << real.reads << "\n";
}
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. The decorator and the proxy derive from the abstract class and hold a reference to the sensor. The two reads of the proxy are in separate statements, so their order is clear.
