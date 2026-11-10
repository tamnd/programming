```cpp run id=checkout
#include <functional>
#include <iostream>
#include <span>
#include <string>
#include <utility>
#include <vector>

struct Item {
    std::string name;
    int price;  // in cents
    int quantity;
};

using Pricing = std::function<int(std::span<const Item>)>;

// The dynamic form: the strategy is a value, and it can change at run time.
class Checkout {
public:
    explicit Checkout(Pricing pricing) : pricing_(std::move(pricing)) {}
    void set_pricing(Pricing pricing) { pricing_ = std::move(pricing); }
    int total(std::span<const Item> items) const { return pricing_(items); }

private:
    Pricing pricing_;
};

// The static form: the strategy is a template parameter, fixed at compile time.
template <typename Strategy>
int total_with(Strategy strategy, std::span<const Item> items) {
    return strategy(items);
}

int regular(std::span<const Item> items) {
    int total = 0;
    for (const auto& item : items) {
        total += item.price * item.quantity;
    }
    return total;
}

int ten_percent_off(std::span<const Item> items) { return regular(items) * 90 / 100; }

// 50 cents off each unit of a line with 5 or more units.
int bulk(std::span<const Item> items) {
    int total = 0;
    for (const auto& item : items) {
        total += (item.quantity >= 5 ? item.price - 50 : item.price) * item.quantity;
    }
    return total;
}

int main() {
    const std::vector<Item> cart{{"book", 1500, 2}, {"pen", 200, 5}};
    const std::vector<std::pair<std::string, Pricing>> strategies{
        {"regular", regular},
        {"ten percent off", ten_percent_off},
        {"bulk", bulk},
    };
    Checkout checkout{regular};
    for (const auto& [name, pricing] : strategies) {
        checkout.set_pricing(pricing);
        std::cout << name << ": " << checkout.total(cart) << " cents\n";
    }
    std::cout << "static bulk: " << total_with(bulk, cart) << " cents\n";
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
static bulk: 3750 cents
```

- Verdict: idiom. C++ has both forms. `std::function` holds any callable with the right signature, so the strategy can change at run time. A template parameter fixes it at compile time, as the `Compare` parameter of `std::sort` does.
- The static form lets the compiler inline the call. Each strategy type makes its own copy of the template.
- A strategy class with virtual functions is the class form, for a strategy with several operations. Lambdas cover the common case of one operation.
