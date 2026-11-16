```cpp run id=static-data-and-functions
#include <iostream>
#include <string>

class Order {
public:
    static constexpr double tax_rate = 0.2; // a constant static data member
    static inline int created = 0;          // one variable for the class

    explicit Order(int amount) : id_(++created), amount_(amount) {}

    static Order from_text(const std::string &text) { // no this pointer
        return Order(std::stoi(text));
    }

    int id() const { return id_; }
    double total() const { return amount_ * (1 + tax_rate); }

private:
    int id_;
    int amount_;
};

int main() {
    Order a(100);
    Order b = Order::from_text("50");
    std::cout << a.id() << " " << b.id() << " " << Order::created << "\n";
    std::cout << a.total() << " " << a.created << "\n";
}
```

```output for=static-data-and-functions
1 2 2
120 2
```

- Verdict: native. A `static` data member is one variable for the class, and a `static` member function has no `this` pointer.
- `inline` lets the class definition also define the variable. Without it, one source file must define `int Order::created = 0;`, or the program does not link.
- `a.created` also works and reads the same variable as `Order::created`; the object is not used.
