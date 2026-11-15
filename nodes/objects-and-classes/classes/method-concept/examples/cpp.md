```cpp run id=member-function-pointers
#include <functional>
#include <iostream>

class Rectangle {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}
    int area() const { return width_ * height_; } // this has the type const Rectangle*
    void scale(int factor) { width_ *= factor; height_ *= factor; }

private:
    int width_;
    int height_;
};

int main() {
    Rectangle r(3, 4);
    int (Rectangle::*unbound)() const = &Rectangle::area; // a pointer to a member function
    auto bound = [&r] { return r.area(); };               // a lambda keeps a reference to r
    r.scale(2);
    std::cout << r.area() << ' ' << (r.*unbound)() << ' ' << bound() << '\n';
    std::cout << std::invoke(&Rectangle::area, r) << '\n';
}
```

```output for=member-function-pointers
48 48 48
48
```

- Verdict: native. C++ calls a method a member function. Inside it, `this` points to the object, and `width_` means `this->width_`. A `const` after the parameter list makes `this` a pointer to `const`, so the function can be called on a `const` object and cannot change it.
- `&Rectangle::area` is a pointer to a member function. It has no object, and the call `(r.*unbound)()` supplies one. `std::invoke` calls it with the object as the first argument. A lambda that captures the object binds it.
