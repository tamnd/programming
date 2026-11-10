```cpp run id=widgets
#include <iostream>
#include <memory>
#include <string>

struct Widget {
    virtual ~Widget() = default;
    virtual std::string render() const = 0;
};

struct WidgetFactory {
    virtual ~WidgetFactory() = default;
    virtual std::unique_ptr<Widget> button(const std::string& label) const = 0;
    virtual std::unique_ptr<Widget> checkbox(const std::string& label, bool checked) const = 0;
};

struct PlainButton : Widget {
    std::string label;
    explicit PlainButton(std::string l) : label(std::move(l)) {}
    std::string render() const override { return "[ " + label + " ]"; }
};

struct PlainCheckbox : Widget {
    std::string label;
    bool checked;
    PlainCheckbox(std::string l, bool c) : label(std::move(l)), checked(c) {}
    std::string render() const override { return std::string("[") + (checked ? "x" : " ") + "] " + label; }
};

struct RoundButton : Widget {
    std::string label;
    explicit RoundButton(std::string l) : label(std::move(l)) {}
    std::string render() const override { return "( " + label + " )"; }
};

struct RoundCheckbox : Widget {
    std::string label;
    bool checked;
    RoundCheckbox(std::string l, bool c) : label(std::move(l)), checked(c) {}
    std::string render() const override { return std::string("(") + (checked ? "*" : " ") + ") " + label; }
};

struct PlainFactory : WidgetFactory {
    std::unique_ptr<Widget> button(const std::string& label) const override {
        return std::make_unique<PlainButton>(label);
    }
    std::unique_ptr<Widget> checkbox(const std::string& label, bool checked) const override {
        return std::make_unique<PlainCheckbox>(label, checked);
    }
};

struct RoundFactory : WidgetFactory {
    std::unique_ptr<Widget> button(const std::string& label) const override {
        return std::make_unique<RoundButton>(label);
    }
    std::unique_ptr<Widget> checkbox(const std::string& label, bool checked) const override {
        return std::make_unique<RoundCheckbox>(label, checked);
    }
};

std::string login_form(const WidgetFactory& widgets) {
    return widgets.button("Save")->render() + " " + widgets.checkbox("Remember me", true)->render();
}

int main() {
    std::cout << "plain: " << login_form(PlainFactory{}) << "\n";
    std::cout << "round: " << login_form(RoundFactory{}) << "\n";
}
```

```output for=widgets
plain: [ Save ] [x] Remember me
round: ( Save ) (*) Remember me
```

- Verdict: idiom. Abstract classes with pure virtual functions. Each factory method returns `std::unique_ptr<Widget>`, so the caller owns the product and the virtual destructor frees it.
- A template parameter can take the place of the abstract factory, as the trait does in Rust. `template <class Factory> std::string login_form(const Factory&)` binds the family at compile time and needs no virtual calls.
- The client takes the factory by reference to `const`. The factories have no state, so one instance of each is enough.
