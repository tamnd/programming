```cpp run id=three-layers
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct Order {
    std::string customer, item;
    int quantity, cents;
};

struct Tables {
    std::map<std::string, int> stock{{"widget", 10}};
    std::vector<Order> orders;
};

// Data source layer: the stored data, and a transaction that keeps all or none of a change.
class Store {
public:
    Tables tables;

    bool transaction(const std::function<void(Tables&)>& change) {
        Tables saved = tables;  // a copy of every table
        try {
            change(tables);
            return true;
        } catch (const std::runtime_error&) {
            tables = saved;  // roll back
            return false;
        }
    }
};

// Domain layer: the business rules, with a special deal for one customer.
constexpr int kPriceCents = 1000;

int price(const std::string& customer, int quantity) {
    int total = kPriceCents * quantity;
    if (customer == "acme" && quantity >= 3) total = total * 90 / 100;  // a deal that sales made with one customer
    return total;
}

bool place_order(Store& store, const std::string& customer, const std::string& item, int quantity) {
    return store.transaction([&](Tables& t) {
        t.orders.push_back({customer, item, quantity, price(customer, quantity)});
        if (t.stock.at(item) < quantity) throw std::runtime_error("not enough stock");
        t.stock[item] -= quantity;
    });
}

std::string money(int cents) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%d.%02d", cents / 100, cents % 100);
    return buf;
}

// Presentation layer: two screens show the same data in different ways.
void order_screen(const Store& store) {
    for (const auto& o : store.tables.orders)
        std::cout << "order screen: " << o.customer << " bought " << o.quantity << " x " << o.item << " for "
                  << money(o.cents) << "\n";
}

void stock_screen(const Store& store) {
    for (const auto& [item, count] : store.tables.stock)
        std::cout << "stock screen: " << item << ": " << count << " in stock, orders: " << store.tables.orders.size()
                  << "\n";
}

int main() {
    Store store;
    std::cout << "acme orders 3: " << (place_order(store, "acme", "widget", 3) ? "kept" : "rolled back") << "\n";
    std::cout << "bolt orders 9: " << (place_order(store, "bolt", "widget", 9) ? "kept" : "rolled back") << "\n";
    order_screen(store);
    stock_screen(store);
}
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. A database client library gives the data, but few teams write a whole enterprise application in C++. The copy of a `Tables` value saves every table for the rollback.
