```cpp run id=checkout
#include <iostream>
#include <map>
#include <string>
#include <utility>

class Widget;

class Mediator {
public:
    virtual void changed(const Widget& widget) = 0;

protected:
    ~Mediator() = default;
};

// A colleague. It knows its mediator and no other widget.
class Widget {
public:
    Widget(std::string name, Mediator& mediator) : name_(std::move(name)), mediator_(mediator) {}
    const std::string& name() const { return name_; }

protected:
    void notify() const { mediator_.changed(*this); }

private:
    std::string name_;
    Mediator& mediator_;
};

class Select : public Widget {
public:
    using Widget::Widget;
    const std::string& value() const { return value_; }
    void choose(const std::string& value) {
        value_ = value;
        std::cout << name() << " = \"" << value << "\"\n";
        notify();
    }

private:
    std::string value_;
};

class TextField : public Widget {
public:
    using Widget::Widget;
    const std::string& text() const { return text_; }
    void type(const std::string& text) {  // the user edits the text
        set_text(text);
        notify();
    }
    void set_text(const std::string& text) {  // the program sets the text: no notification
        text_ = text;
        std::cout << name() << " = \"" << text << "\"\n";
    }

private:
    std::string text_;
};

class Button : public Widget {
public:
    using Widget::Widget;
    void set_enabled(bool enabled) {
        if (enabled == enabled_) return;
        enabled_ = enabled;
        std::cout << name() << (enabled ? " enabled" : " disabled") << '\n';
    }
    void click() const {
        if (enabled_) notify();
    }

private:
    bool enabled_ = false;
};

// The mediator. All rules that join the widgets are here.
class CheckoutForm final : public Mediator {
public:
    CheckoutForm() = default;
    CheckoutForm(const CheckoutForm&) = delete;  // a copy's widgets would refer to this form
    CheckoutForm& operator=(const CheckoutForm&) = delete;

    Select country{"country", *this};
    TextField postcode{"postcode", *this};
    Button submit{"submit", *this};

    void changed(const Widget& widget) override {
        if (&widget == &country) {
            postcode.set_text("");
            submit.set_enabled(false);
        } else if (&widget == &postcode) {
            auto needed = lengths_.find(country.value());
            submit.set_enabled(needed != lengths_.end() && postcode.text().size() == needed->second);
        } else if (&widget == &submit) {
            std::cout << "order sent to " << country.value() << ' ' << postcode.text() << '\n';
        }
    }

private:
    const std::map<std::string, std::size_t> lengths_{{"US", 5}, {"NL", 6}};
};

int main() {
    CheckoutForm form;
    form.country.choose("US");
    form.postcode.type("1234");
    form.postcode.type("12345");
    form.country.choose("NL");
    form.postcode.type("1234AB");
    form.submit.click();
}
```

```output for=checkout
country = "US"
postcode = ""
postcode = "1234"
postcode = "12345"
submit enabled
country = "NL"
postcode = ""
submit disabled
postcode = "1234AB"
submit enabled
order sent to NL 1234AB
```

- Verdict: idiom. An abstract `Mediator`, widgets that hold a `Mediator&`, and a form whose members are the widgets.
- The form passes `*this` to its members as they are built. The widgets only store the reference during construction and do not call it, so the form need not be complete yet.
- The form deletes its copy operations. A copy's widgets would still refer to the old form. With the copy constructor deleted, the compiler does not declare a move constructor either, so a move is a compile error too.
- `&widget == &country` compares addresses, so it is an identity test. The program prints the rules' results and not the addresses.
