```cpp run id=thermometer
#include <cstdio>
#include <string>

class Thermometer {
public:
    virtual ~Thermometer() = default;
    virtual double celsius() const = 0;
};

void report(const std::string& name, const Thermometer& sensor) {
    std::printf("%s: %.1f C\n", name.c_str(), sensor.celsius());
}

class RoomSensor : public Thermometer {
public:
    explicit RoomSensor(double value) : value_(value) {}
    double celsius() const override { return value_; }

private:
    double value_;
};

// The vendor class. The program cannot change it.
class LegacyThermometer {
public:
    explicit LegacyThermometer(int tenths) : tenths_(tenths) {}
    int fahrenheit_tenths() const { return tenths_; }

private:
    int tenths_;
};

// An object adapter: it implements the target and refers to the adaptee.
class LegacyAdapter : public Thermometer {
public:
    explicit LegacyAdapter(const LegacyThermometer& legacy) : legacy_(legacy) {}
    double celsius() const override {
        return (legacy_.fahrenheit_tenths() / 10.0 - 32) * 5 / 9;
    }

private:
    const LegacyThermometer& legacy_;
};

int main() {
    report("lab", RoomSensor(21.5));
    LegacyThermometer patient(986);
    LegacyThermometer hall(704);
    report("patient", LegacyAdapter(patient));
    report("hall", LegacyAdapter(hall));
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. `LegacyAdapter` holds a reference to the adaptee, so the adaptee must live longer than the adapter. Here both live in `main`.
- A class adapter in C++ inherits publicly from the target and privately from the adaptee. It can call the adaptee's methods as its own, but it adapts one concrete class only.
- `std::stack` and `std::queue` are container adaptors. They take an underlying container type and give it a smaller interface.
