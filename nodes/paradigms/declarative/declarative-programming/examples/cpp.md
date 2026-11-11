```cpp run id=regex-ranges
#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Order {
    std::string item;
    int qty;
    int price;
};

int main() {
    std::vector<Order> orders{{"pen", 3, 2}, {"book", 1, 12}, {"ink", 2, 5}};
    std::ranges::sort(orders, std::ranges::greater{}, [](const Order& o) { return o.qty * o.price; });
    for (const auto& o : orders) std::cout << o.item << " ";
    std::cout << "\n";

    const std::string text = "from 2024-01-31 to 2025-12-01";
    const std::regex date(R"((\d{4})-(\d{2})-(\d{2}))");
    for (auto it = std::sregex_iterator(text.begin(), text.end(), date); it != std::sregex_iterator(); ++it) {
        std::cout << (*it)[1] << "\n";
    }
}
```

```output for=regex-ranges
book ink pen 
2024
2025
```

- Verdict: idiom. The call to `std::ranges::sort` states the order with a comparison and a projection, and the library chooses the algorithm. `<regex>` provides regular expressions, though it is slow compared with other engines.
