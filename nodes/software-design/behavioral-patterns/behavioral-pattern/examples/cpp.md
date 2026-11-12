```cpp run id=kinds
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

class Checkout {
public:
    virtual ~Checkout() = default;
    int total(const std::vector<int>& prices) const {
        return discount(std::accumulate(prices.begin(), prices.end(), 0));
    }
protected:
    virtual int discount(int amount) const { return amount; }
};

class SaleCheckout : public Checkout {
protected:
    int discount(int amount) const override { return amount * 9 / 10; }
};

struct Till {
    std::function<int(int)> discount;
    int total(const std::vector<int>& prices) const {
        return discount(std::accumulate(prices.begin(), prices.end(), 0));
    }
};

int main() {
    std::vector<int> prices{40, 60};
    std::cout << "class pattern: " << SaleCheckout{}.total(prices) << "\n";
    Till till{[](int amount) { return amount * 9 / 10; }};
    std::cout << "object pattern: " << till.total(prices) << "\n";
    till.discount = [](int amount) { return amount - 20; };
    std::cout << "changed at run time: " << till.total(prices) << "\n";
}
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: idiom. The class pattern uses a virtual function. The strategy is a `std::function` that holds a lambda.
