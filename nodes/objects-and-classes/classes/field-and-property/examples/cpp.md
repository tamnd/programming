```cpp run id=data-members
#include <iostream>
#include <stdexcept>

struct Size {
    int width = 0; // public data members with default member initializers
    int height = 0;
};

class Rectangle {
public:
    Rectangle(int width, int height) : height_(height) { set_width(width); }

    int width() const { return width_; } // an accessor
    void set_width(int width) {
        if (width < 0) {
            throw std::invalid_argument("negative width");
        }
        width_ = width;
    }
    int area() const { return width_ * height_; } // computed on each call

private:
    int width_ = 0; // private data members
    int height_ = 0;
};

int main() {
    Size s{3, 4};
    s.width = 5; // any code can change a public member
    std::cout << s.width << ' ' << s.height << '\n';

    Rectangle r(3, 4);
    std::cout << r.width() << ' ' << r.area() << '\n';
    r.set_width(5);
    std::cout << r.area() << '\n';
    try {
        r.set_width(-1);
    } catch (const std::invalid_argument &e) {
        std::cout << e.what() << '\n';
    }
    std::cout << sizeof(Rectangle) << ' ' << sizeof(Size) << '\n';
}
```

```output for=data-members
5 4
3 12
20
negative width
8 8
```

- Verdict: idiom. C++ calls fields data members and has no property syntax. A plain aggregate such as `Size` uses public members, and a class with an invariant keeps them private and adds accessor functions.
- The accessors are ordinary member functions. A short accessor defined in the class body is inline, so it usually costs nothing after optimization.
- The accessors add no storage: `Rectangle` and `Size` both hold two `int` values.
