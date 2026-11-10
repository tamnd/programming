```cpp run id=sensor
#include <cstdio>
#include <functional>
#include <map>
#include <utility>

class Sensor {
public:
    using Slot = std::function<void(double)>;

    // Ends the subscription when it goes out of scope.
    class Subscription {
    public:
        Subscription(Sensor* sensor, int id) : sensor_(sensor), id_(id) {}
        Subscription(Subscription&& other) noexcept
            : sensor_(std::exchange(other.sensor_, nullptr)), id_(other.id_) {}
        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;
        Subscription& operator=(Subscription&&) = delete;
        ~Subscription() {
            if (sensor_ != nullptr) {
                sensor_->slots_.erase(id_);
            }
        }

    private:
        Sensor* sensor_;
        int id_;
    };

    [[nodiscard]] Subscription subscribe(Slot slot) {
        int id = next_id_++;
        slots_.emplace(id, std::move(slot));
        return Subscription(this, id);
    }

    void set(double value) {
        auto copy = slots_;  // a slot can unsubscribe during the loop
        for (auto& [id, slot] : copy) {
            slot(value);
        }
    }

private:
    std::map<int, Slot> slots_;
    int next_id_ = 0;
};

int main() {
    Sensor sensor;
    auto alarm = sensor.subscribe([](double value) {
        if (value > 30) {
            std::printf("alarm: %.1f is above 30\n", value);
        }
    });
    {
        auto display = sensor.subscribe([](double value) { std::printf("display: %.1f\n", value); });
        sensor.set(21.5);
        sensor.set(30.5);
    }
    sensor.set(31.0);
}
```

```output for=sensor
display: 21.5
alarm: 30.5 is above 30
display: 30.5
alarm: 31.0 is above 30
```

- Verdict: idiom. The C++ standard library has no observer or signal type. A container of `std::function` slots is the usual hand-written form. Libraries such as Boost.Signals2 and Qt signals and slots add thread safety and automatic disconnection.
- `subscribe` returns a move-only `Subscription`. Its destructor removes the slot, so the display stops at the end of its block. This ties the life of the subscription to a scope, as RAII does for memory and files.
- The keys of the `std::map` grow with each subscription, so the slots run in the order of subscription. Here the alarm subscribed first, so its line comes first.
- The subject must outlive each `Subscription`, because the destructor uses the pointer to the sensor. A subject that can die first needs shared ownership, for example a `std::weak_ptr` in the subscription.
