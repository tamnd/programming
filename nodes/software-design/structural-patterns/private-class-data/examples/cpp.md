```cpp run id=order-data
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct OrderData {
    int order_id;
    std::string customer;
};

class Order {
public:
    Order(int order_id, std::string customer, std::vector<std::string> items)
        : data_{order_id, std::move(customer)}, items_(std::move(items)) {}

    int order_id() const { return data_.order_id; }
    const std::string& customer() const { return data_.customer; }
    void add(std::string item) { items_.push_back(std::move(item)); }

    std::string describe() const {
        return "order " + std::to_string(order_id()) + " for " + customer() + ": " +
               std::to_string(items_.size()) + " items";
    }

private:
    const OrderData data_;  // fixed after construction, also for the methods of Order
    std::vector<std::string> items_;  // changes
};

int main() {
    Order order(42, "ann", {"pen", "ink"});
    std::cout << order.describe() << "\n";
    order.add("paper");
    std::cout << order.describe() << "\n";
}
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. A `const` member can be set only in the constructor's initializer list, so an assignment in any method does not compile.
