```cpp run id=orders
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

enum class Status { open, shipped };

const char* name(Status s) { return s == Status::open ? "open" : "shipped"; }

struct Order {
    int id;
    std::string customer;
    int total;
    Status status = Status::open;

    void ship() {
        if (status != Status::open)
            throw std::logic_error("order " + std::to_string(id) + " is " + name(status));
        status = Status::shipped;
    }
};

class OrderRepository {
public:
    virtual ~OrderRepository() = default;
    virtual void add(const Order& order) = 0;
    virtual std::optional<Order> get(int id) const = 0;
    virtual std::vector<Order> by_customer(const std::string& customer) const = 0;
    virtual void update(const Order& order) = 0;
};

// Returns values, not references, so callers cannot change stored rows.
class InMemoryOrderRepository : public OrderRepository {
public:
    void add(const Order& order) override {
        if (!rows_.emplace(order.id, order).second)
            throw std::invalid_argument("order " + std::to_string(order.id) + " exists");
    }
    std::optional<Order> get(int id) const override {
        auto it = rows_.find(id);
        if (it == rows_.end()) return std::nullopt;
        return it->second;
    }
    std::vector<Order> by_customer(const std::string& customer) const override {
        std::vector<Order> out;
        for (const auto& [id, o] : rows_)
            if (o.customer == customer) out.push_back(o);
        return out;
    }
    void update(const Order& order) override {
        auto it = rows_.find(order.id);
        if (it == rows_.end()) throw std::out_of_range("order " + std::to_string(order.id));
        it->second = order;
    }

private:
    std::map<int, Order> rows_;
};

std::string ship_order(OrderRepository& orders, int id) {
    auto order = orders.get(id);
    if (!order) return "order " + std::to_string(id) + ": not found";
    order->ship();
    orders.update(*order);
    return "order " + std::to_string(id) + " is " + name(order->status);
}

int main() {
    InMemoryOrderRepository repo;
    repo.add({1, "ana", 30});
    repo.add({2, "bo", 12});
    repo.add({3, "ana", 5});
    std::cout << "ana: ";
    const char* sep = "";
    for (const auto& o : repo.by_customer("ana")) {
        std::cout << sep << "order " << o.id << " (" << o.total << ")";
        sep = ", ";
    }
    std::cout << "\n" << ship_order(repo, 1) << "\n" << ship_order(repo, 9) << "\n";
    auto stored = repo.get(1);
    std::cout << "stored: order " << stored->id << " is " << name(stored->status) << "\n";
}
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. An abstract class gives the contract, and `std::optional<Order>` makes "not found" part of the return type.
- `get` returns an `Order` by value. A version that returns `Order&` or `Order*` into the map would let callers change stored rows without `update`, and the pointer would dangle if the map erased the row.
- `std::map` keeps the keys sorted, so `by_customer` gives orders sorted by id.
