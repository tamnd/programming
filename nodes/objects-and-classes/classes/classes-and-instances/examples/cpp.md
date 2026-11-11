```cpp run id=class-and-struct
#include <iostream>
#include <string>
#include <utility>

class Account {
public:
    static constexpr double rate = 0.02; // static: one value for the class

    Account(std::string owner, long balance) : owner_(std::move(owner)), balance_(balance) { ++opened_; }

    void deposit(long amount) { balance_ += amount; }
    double interest() const { return balance_ * rate; } // const: does not change the object
    static int opened() { return opened_; }

    friend std::ostream& operator<<(std::ostream& os, const Account& a)
    {
        return os << "Account(" << a.owner_ << ", " << a.balance_ << ")";
    }

private: // members of a class are private unless declared otherwise
    std::string owner_;
    long balance_;
    static inline int opened_ = 0;
};

struct Point { // members of a struct are public by default
    int x, y;
};

int main()
{
    Account a{"ana", 100};
    Account b{"bo", 0};
    a.deposit(50);
    std::cout << a << ' ' << b << ' ' << Account::opened() << '\n';
    std::cout << a.interest() << '\n';
    Point p{1, 2};
    Point q = p; // objects are values: q is a copy
    q.x = 9;
    std::cout << p.x << ' ' << q.x << '\n';
}
```

```output for=class-and-struct
Account(ana, 150) Account(bo, 0) 2
3
1 9
```

```cpp wrong id=object-slicing
#include <iostream>
#include <string>

struct Shape {
    virtual ~Shape() = default;
    virtual std::string name() const { return "shape"; }
};

struct Circle : Shape {
    double r = 1;
    std::string name() const override { return "circle"; }
};

void print(Shape s) { std::cout << s.name() << '\n'; } // takes a Shape by value

int main()
{
    Circle c;
    print(c); // copies only the Shape part of c: the object is sliced
    const Shape& ref = c;
    std::cout << ref.name() << '\n';
}
```

```output for=object-slicing
shape
circle
```

- Verdict: native. `class` and `struct` declare the same kind of type and differ only in the default access: members of a `class` are private and members of a `struct` are public. Static members belong to the class, and a `const` member function promises not to change the object. Objects are values: assigning one copies its members, as `q = p` shows, and a pointer or reference is needed to share one object.
- The wrong block passes a `Circle` to a function that takes a `Shape` by value. The copy keeps only the `Shape` part of the object, so the call reaches `Shape::name`. This is called slicing. Pass objects of a class hierarchy by reference or by pointer, as the second call does.
