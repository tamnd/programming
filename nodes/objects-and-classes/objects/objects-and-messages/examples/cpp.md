```cpp run id=storage-and-lifetime
#include <iostream>
#include <memory>
#include <string>

struct Point {
    int x, y;
};

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
};

int counter() {
    static int calls = 0; // one object for the whole program
    return ++calls;
}

int main() {
    int n = 5;              // an object with automatic storage
    auto p = std::make_unique<Point>(Point{1, 2}); // an object with dynamic storage
    Point q = *p;           // a new object with the same value
    q.x = 9;
    std::cout << sizeof n << ' ' << alignof(double) << ' ' << p->x << ' ' << q.x << '\n';

    int& r = n; // a reference is a name for an object, not an object
    r = 6;
    std::cout << n << ' ' << (&r == &n) << '\n';

    counter();
    std::cout << counter() << '\n';

    std::unique_ptr<Shape> s = std::make_unique<Square>(3);
    std::cout << s->area() << ' ' << sizeof(Square) / sizeof(void*) << '\n';
}
```

```output for=storage-and-lifetime
4 8 1 9
6 1
2
9 2
```

- Verdict: native. In C++ an object is a region of storage with a type, a size, an alignment, a storage duration, a lifetime and a value. An `int` is an object as much as an instance of a class is. Objects have automatic storage in a block, static storage for the whole program, as `calls` does, or dynamic storage from `new` or `make_unique`.
- Objects are values: `Point q = *p` creates a second object with the same value. A reference is another name for an existing object and is not an object itself. An object of a class with virtual functions holds a hidden pointer to the table of virtual functions, which is why a `Square` holding one `double` is two words wide on common platforms.
