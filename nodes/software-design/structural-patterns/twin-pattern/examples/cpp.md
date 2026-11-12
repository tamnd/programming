```cpp run id=twin-ball
#include <iostream>
#include <string>
#include <utility>

class Item {
public:
    explicit Item(std::string name) : name_(std::move(name)) {}
    virtual ~Item() = default;
    void show() const { std::cout << name_ << " at " << position() << "\n"; }

private:
    virtual int position() const = 0;
    std::string name_;
};

class Ticker {
public:
    virtual ~Ticker() = default;
    void tick() {
        ++count_;
        on_tick();
    }
    int count() const { return count_; }

private:
    virtual void on_tick() = 0;
    int count_ = 0;
};

// C++ has multiple inheritance, so one class has both parents.
class Ball : public Item, public Ticker {
public:
    explicit Ball(std::string name) : Item(std::move(name)) {}

private:
    int position() const override { return x_; }
    void on_tick() override { x_ += 2; }
    int x_ = 0;
};

int main() {
    Ball ball("ball");
    ball.show();
    for (int i = 0; i < 2; ++i) {
        ball.tick();
        ball.show();
    }
    std::cout << "ticks: " << ball.count() << "\n";
}
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: native. C++ has multiple inheritance, so `Ball` derives from both `Item` and `Ticker`, and a twin is not needed.
