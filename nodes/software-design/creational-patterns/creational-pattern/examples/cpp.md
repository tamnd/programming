```cpp run id=language-features
#include <iostream>
#include <memory>
#include <string>

struct Shape {
    virtual ~Shape() = default;
    virtual std::unique_ptr<Shape> clone() const = 0;  // the prototype operation
    virtual std::string name() const = 0;
    int size = 1;
};

struct Square : Shape {
    std::unique_ptr<Shape> clone() const override { return std::make_unique<Square>(*this); }
    std::string name() const override { return "Square"; }
};

struct Registry {
    int lookups = 0;
};

Registry& registry() {
    static Registry instance;  // made once, on the first call
    return instance;
}

struct Request {
    std::string url;
    std::string method = "GET";
    int retries = 0;
};

int main() {
    // std::make_unique is a factory function that forwards its arguments.
    std::unique_ptr<Shape> shape = std::make_unique<Square>();
    shape->size = 3;

    // A virtual clone copies an object through a pointer to its base class.
    std::unique_ptr<Shape> copy = shape->clone();
    std::cout << "prototype: " << copy->name() << " " << copy->size << " "
              << std::boolalpha << (copy != shape) << "\n";

    ++registry().lookups;
    ++registry().lookups;
    std::cout << "singleton: " << registry().lookups << " lookups on one instance\n";

    // Designated initializers and default member values replace a small builder.
    Request r{.url = "https://example.com/", .retries = 2};
    std::cout << "initializer: " << r.url << " " << r.method << " " << r.retries << "\n";
}
```

```output
prototype: Square 3 true
singleton: 2 lookups on one instance
initializer: https://example.com/ GET 2
```

- Verdict: idiom. `std::make_unique` and `std::make_shared` are factory functions. A copy constructor copies an object of a known class, and a virtual `clone` copies through a base pointer, which is the Prototype pattern. A function-local `static` is a lazy singleton. Designated initializers with default member values replace a builder for a plain struct.
