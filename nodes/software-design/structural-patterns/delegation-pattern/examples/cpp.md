```cpp run id=window-shape
#include <iostream>
#include <memory>
#include <string>

struct Titled {
    virtual ~Titled() = default;
    virtual std::string title() const = 0;
};

struct Shape {
    virtual ~Shape() = default;
    virtual int area() const = 0;
    virtual std::string describe(const Titled& owner) const = 0;
};

struct Rectangle : Shape {
    int width, height;
    Rectangle(int w, int h) : width(w), height(h) {}
    int area() const override { return width * height; }
    std::string describe(const Titled& owner) const override {
        return owner.title() + ": a rectangle of " + std::to_string(width) + " by " + std::to_string(height);
    }
};

struct Triangle : Shape {
    int base, height;
    Triangle(int b, int h) : base(b), height(h) {}
    int area() const override { return base * height / 2; }
    std::string describe(const Titled& owner) const override {
        return owner.title() + ": a triangle of " + std::to_string(base) + " by " + std::to_string(height);
    }
};

class AppWindow : public Titled {
public:
    AppWindow(std::string title, std::unique_ptr<Shape> shape)
        : title_(std::move(title)), shape_(std::move(shape)) {}
    std::string title() const override { return title_; }
    void set_shape(std::unique_ptr<Shape> shape) { shape_ = std::move(shape); }
    int area() const { return shape_->area(); }  // forward
    std::string describe() const { return shape_->describe(*this); }  // delegate, and pass the receiver

private:
    std::string title_;
    std::unique_ptr<Shape> shape_;  // the delegate
};

int main() {
    AppWindow w("main", std::make_unique<Rectangle>(3, 4));
    std::cout << "area: " << w.area() << "\n" << w.describe() << "\n";
    w.set_shape(std::make_unique<Triangle>(3, 4));  // change the behaviour at run time
    std::cout << "area: " << w.area() << "\n" << w.describe() << "\n";
}
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: idiom. The window owns its delegate through a `std::unique_ptr`, and replacing the pointer changes the behaviour.
