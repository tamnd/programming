```cpp run id=member-initializers
#include <iostream>
#include <stdexcept>
#include <string>

struct Trace {
    explicit Trace(const char* what) { std::cout << "init " << what << '\n'; }
};

class Range {
public:
    Range(int lo, int hi) : lo_(check(lo, hi)), hi_(hi)  // a member initializer list
    {
        std::cout << "constructor body\n";
    }

    explicit Range(int hi) : Range(0, hi) {}  // a delegating constructor

    static Range parse(const std::string& text)  // a named constructor
    {
        auto dots = text.find("..");
        return Range(std::stoi(text.substr(0, dots)), std::stoi(text.substr(dots + 2)));
    }

    int size() const { return hi_ - lo_ + 1; }

private:
    static int check(int lo, int hi)
    {
        if (lo > hi) throw std::invalid_argument("empty range");
        return lo;
    }

    Trace first_{"first_"};  // members initialize in the order of declaration
    int lo_;
    int hi_;
    Trace last_{"last_"};
};

int main()
{
    std::cout << Range(3, 7).size() << '\n';
    std::cout << Range::parse("10..12").size() << '\n';
    try {
        Range(9, 1);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << '\n';
    }
}
```

```output for=member-initializers
init first_
init last_
constructor body
5
init first_
init last_
constructor body
3
init first_
empty range
```

```cpp wrong id=virtual-call-in-constructor
#include <iostream>
#include <string>

class Widget {
public:
    Widget() { render(); }  // a virtual call during construction
    virtual ~Widget() = default;
    virtual void render() const { std::cout << "widget\n"; }
};

class Label : public Widget {
public:
    Label() : text_("hello") {}
    void render() const override { std::cout << "label: " << text_ << '\n'; }

private:
    std::string text_;
};

int main()
{
    Label l;     // Widget() runs while the object is still a Widget
    l.render();  // after construction, the call reaches Label
}
```

```output for=virtual-call-in-constructor
widget
label: hello
```

- Verdict: native. A constructor initializes the bases and members in its member initializer list, and then runs its body. The order is fixed by the class, not by the list: bases first, then members in the order of their declaration, which the traces of `first_` and `last_` show. A constructor can delegate to another, and `explicit` stops a one-argument constructor from acting as an implicit conversion. If a constructor throws, the members already built are destroyed and the object never exists, so `Range(9, 1)` builds `first_` and then stops.
- The wrong block calls a virtual function from a base constructor. While `Widget`'s constructor runs, the object is a `Widget`, so the call reaches `Widget::render`, not the override. After construction, the same call reaches `Label::render`. The behavior is defined, unlike in Java and JavaScript, but it still surprises readers who expect the override.
