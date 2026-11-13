```cpp run id=transaction-script
#include <cstdio>
#include <iostream>
#include <map>
#include <string>

struct Customer {
    std::string name;
    int credit;
};

struct Order {
    int customer;
    int cents;
    std::string state = "open";
};

// A thin wrapper over the stored tables. The scripts call it directly.
std::map<int, Customer> customers{{1, {"ada", 5000}}};
std::map<int, Order> orders;

Customer& find_customer(int customer_id) { return customers.at(customer_id); }

int insert_order(int customer_id, int cents) {
    int order_id = static_cast<int>(orders.size()) + 1;
    orders[order_id] = Order{customer_id, cents};
    return order_id;
}

std::string money(int cents) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%d.%02d", cents / 100, cents % 100);
    return buf;
}

// One transaction script for each request from the presentation.
std::string place_order(int customer_id, int cents) {
    Customer& customer = find_customer(customer_id);
    if (customer.credit < cents) return "refused, credit left " + money(customer.credit);
    customer.credit -= cents;
    return "order " + std::to_string(insert_order(customer_id, cents));
}

std::string cancel_order(int order_id) {
    Order& order = orders.at(order_id);
    if (order.state != "open") return "already cancelled";
    order.state = "cancelled";
    find_customer(order.customer).credit += order.cents;
    return "refunded " + money(order.cents);
}

int main() {
    std::cout << "place_order(ada, 30.00): " << place_order(1, 3000) << "\n";
    std::cout << "place_order(ada, 30.00): " << place_order(1, 3000) << "\n";
    std::cout << "cancel_order(1): " << cancel_order(1) << "\n";
    std::cout << "cancel_order(1): " << cancel_order(1) << "\n";
    std::cout << "place_order(ada, 30.00): " << place_order(1, 3000) << "\n";
    std::cout << "credit of ada: " << money(find_customer(1).credit) << "\n";
}
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a free function, and the finders return references into `std::map` tables.
