```cpp run id=invoice
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Item {
    std::string name;
    int price;
    int quantity;
};

std::string invoice_before(const std::string& customer, const std::vector<Item>& items) {
    std::string text = std::format("Invoice for {}\n", customer);
    for (const auto& item : items) {
        text += std::format("  {}: {} x {}.{:02}\n", item.name, item.quantity, item.price / 100, item.price % 100);
    }
    // compute the total
    int total = 0;
    for (const auto& item : items) {
        total += item.price * item.quantity;
    }
    text += std::format("Total: {}.{:02}", total / 100, total % 100);
    return text;
}

std::string format_cents(int cents) {
    return std::format("{}.{:02}", cents / 100, cents % 100);
}

int total_cents(const std::vector<Item>& items) {
    int total = 0;
    for (const auto& item : items) {
        total += item.price * item.quantity;
    }
    return total;
}

std::string invoice_after(const std::string& customer, const std::vector<Item>& items) {
    std::string text = std::format("Invoice for {}\n", customer);
    for (const auto& item : items) {
        text += std::format("  {}: {} x {}\n", item.name, item.quantity, format_cents(item.price));
    }
    text += "Total: " + format_cents(total_cents(items));
    return text;
}

int main() {
    const std::vector<Item> items{{"pen", 250, 2}, {"pad", 1205, 1}};
    const std::string expected = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05";
    using Invoice = std::string (*)(const std::string&, const std::vector<Item>&);
    const std::pair<const char*, Invoice> versions[] = {{"invoice_before", invoice_before},
                                                        {"invoice_after", invoice_after}};
    for (const auto& [name, invoice] : versions) {
        if (invoice("Ada", items) != expected || invoice("Bob", {}) != "Invoice for Bob\nTotal: 0.00") {
            throw std::runtime_error(std::string(name) + " failed");
        }
        std::cout << name << ": tests pass\n";
    }
    std::cout << invoice_after("Ada", items) << '\n';
}
```

```output for=invoice
invoice_before: tests pass
invoice_after: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring makes a free function, or a member function when the fragment uses the members of a class.
- Pass a value that the fragment only reads as a `const` reference, and return a value that it computes. A non-const reference parameter also works for a variable that the fragment changes, but it hides the data flow at the call. clangd uses a reference for such a variable when it extracts.
- A fragment that computes two values can return a struct or a `std::pair`, and the caller unpacks it with a structured binding: `auto [count, total] = add_up(items);`.
- A function in the same translation unit that is small can be inlined by the optimizer, so the extraction need not cost a call at run time.
