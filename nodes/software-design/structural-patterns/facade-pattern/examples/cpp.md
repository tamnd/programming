```cpp run id=shop
#include <format>
#include <iostream>
#include <map>
#include <string>

namespace detail {  // the subsystem; by convention, clients do not use detail

class Inventory {
public:
    explicit Inventory(std::map<std::string, int> stock) : stock_(std::move(stock)) {}
    bool reserve(const std::string& item) {
        auto it = stock_.find(item);
        if (it == stock_.end() || it->second == 0) return false;
        --it->second;
        return true;
    }
    void release(const std::string& item) { ++stock_[item]; }
    int count(const std::string& item) const {
        auto it = stock_.find(item);
        return it == stock_.end() ? 0 : it->second;
    }

private:
    std::map<std::string, int> stock_;
};

struct Payments {
    bool charge(const std::string& card, double) const { return card != "expired"; }
};

class Shipping {
public:
    int schedule(const std::string&) { return ++parcels_; }

private:
    int parcels_ = 0;
};

}  // namespace detail

// The facade.
class Shop {
public:
    explicit Shop(std::map<std::string, int> stock) : inventory_(std::move(stock)) {}

    std::string place_order(const std::string& item, const std::string& card, double price) {
        if (!inventory_.reserve(item)) return std::format("failed: {} is out of stock", item);
        if (!payments_.charge(card, price)) {
            inventory_.release(item);
            return std::format("failed: card declined, {} back in stock", item);
        }
        return std::format("ok: {}, paid {:.2f}, parcel {}", item, price, shipping_.schedule(item));
    }

    int stock(const std::string& item) const { return inventory_.count(item); }

private:
    detail::Inventory inventory_;
    detail::Payments payments_;
    detail::Shipping shipping_;
};

int main() {
    Shop shop({{"book", 2}, {"lamp", 0}});
    std::cout << shop.place_order("book", "visa", 12.50) << "\n";
    std::cout << shop.place_order("book", "expired", 12.50) << "\n";
    std::cout << shop.place_order("lamp", "visa", 30.00) << "\n";
    std::cout << "stock of book: " << shop.stock("book") << "\n";
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. `Shop` is the facade, and it holds the three subsystem objects by value.
- C++ has no private namespaces. A `detail` namespace marks the subsystem as internal only by convention. A library hides it for real by keeping its header out of the public include directory.
- A C++ module can export `Shop` and not export the subsystem classes. Then a program that imports the module cannot name them.
