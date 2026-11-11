```cpp run id=destructor-order
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Noisy {
    std::string name;
    explicit Noisy(std::string n) : name(std::move(n)) { std::cout << "construct " << name << '\n'; }
    ~Noisy() { std::cout << "destroy " << name << '\n'; }
};

struct Base {
    virtual ~Base() { std::cout << "~Base\n"; } // virtual: delete through Base* is safe
};

struct Derived : Base {
    Noisy member{"member"};
    ~Derived() override { std::cout << "~Derived\n"; }
};

int main()
{
    Noisy a{"a"};
    {
        Noisy b{"b"};
        Noisy c{"c"};
        std::cout << "end of block\n";
    } // reverse order of construction: c, then b
    {
        std::unique_ptr<Base> p = std::make_unique<Derived>();
        std::cout << "end of block\n";
    } // ~Derived, then its members, then ~Base
    std::cout << "end of main\n";
}
```

```output for=destructor-order
construct a
construct b
construct c
end of block
destroy c
destroy b
construct member
end of block
~Derived
destroy member
~Base
end of main
destroy a
```

- Verdict: native. A destructor runs when an object's lifetime ends: at the end of the scope for a local, at `delete` or the destruction of a smart pointer for a heap object, and when the owner is destroyed for a member. Objects in one scope are destroyed in the reverse order of their construction. Inside an object, the body of the destructor runs first, then the members in reverse order, then the base classes.
- A class that is deleted through a pointer to its base needs a virtual destructor in the base. Without one, `delete` on a `Base*` that points to a `Derived` is undefined behavior: in practice `~Derived` and the destructors of its members do not run. `std::unique_ptr<Base>` has the same need.
