```cpp run id=pimpl
#include <iostream>
#include <memory>
#include <string>

// The header: Widget holds only a pointer to an incomplete type.
class Widget {
public:
    explicit Widget(std::string name);
    ~Widget();
    void click();
    std::string describe() const;

private:
    struct Impl; // declared, not defined
    std::unique_ptr<Impl> impl_;
};

// The source file: the members of Impl can change without a change to the header.
struct Widget::Impl {
    std::string name;
    int clicks = 0;
};

Widget::Widget(std::string name) : impl_(std::make_unique<Impl>()) {
    impl_->name = std::move(name);
}

Widget::~Widget() = default; // Impl is complete here

void Widget::click() { impl_->clicks++; }

std::string Widget::describe() const {
    return impl_->name + " " + std::to_string(impl_->clicks);
}

int main() {
    Widget w("ok");
    w.click();
    w.click();
    std::cout << w.describe() << "\n";
    std::cout << (sizeof(Widget) == sizeof(void*)) << "\n";
}
```

```output for=pimpl
ok 2
1
```

- Verdict: native. `Widget` holds a `std::unique_ptr` to `Impl`, which the header declares but does not define, so the size of `Widget` is one pointer, and code that includes the header does not see the members. The destructor is defined after `Impl` is complete, because `unique_ptr` must know the type to delete it.
