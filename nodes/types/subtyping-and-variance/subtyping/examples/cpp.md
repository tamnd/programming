```cpp run id=hierarchy
#include <iostream>
#include <memory>
#include <vector>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual Shape* clone() const = 0;
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
    Square* clone() const override { return new Square(*this); }
};

struct Named { const char* name = "named"; };
struct Tile : Square, Named {
    explicit Tile(double s) : Square(s) {}
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Square>(2));
    shapes.push_back(std::make_unique<Tile>(3));
    for (const auto& s : shapes) std::cout << s->area() << "\n";

    Square square(4);
    std::unique_ptr<Square> copy(square.clone());
    std::cout << copy->side << "\n";

    Tile tile(1);
    void* whole = &tile;
    std::cout << (static_cast<void*>(static_cast<Square*>(&tile)) == whole) << "\n";
    std::cout << (static_cast<void*>(static_cast<Named*>(&tile)) == whole) << "\n";
}
```

```output for=hierarchy
4
9
4
1
0
```

- Verdict: native. Subtyping is nominal, through public base classes. A virtual function dispatches on the dynamic type.
- `Square::clone` returns `Square*` where `Shape::clone` returns `Shape*`. This is a covariant return type, so the copy needs no cast.
- A pointer to `Tile` converts to a pointer to its first base, `Square`, at the same address. The conversion to the second base, `Named`, changes the address.
- Pass a polymorphic object by reference or by pointer. A copy by value slices it, as the concept page shows.
