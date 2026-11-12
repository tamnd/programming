```cpp run id=forward-declarations-and-odr
#include <iostream>
#include <string>

struct Node;                   // a forward declaration: an incomplete type
int twice(int x);              // a function declaration
extern int counter;            // a variable declaration

struct List {
    Node* head = nullptr;      // a pointer to an incomplete type is valid
    static int made;           // a declaration of a static data member
    static inline int open = 0;  // an inline variable: also a definition
};
int List::made = 0;            // the definition of the static data member

auto greet(const std::string& n) { return "hi " + n; }  // the return type is deduced

int main() {
    std::cout << "twice(4) = " << twice(4) << "\n";
    counter += 1;
    List::made += 1;
    std::cout << "counter = " << counter << ", made = " << List::made
              << ", open = " << List::open << "\n";
    std::cout << greet("ada") << "\n";
}

int counter = 41;              // the definition of counter
int twice(int x) { return 2 * x; }
```

```output for=forward-declarations-and-odr
twice(4) = 8
counter = 42, made = 1, open = 0
hi ada
```

- Verdict: native. `struct Node;` is a forward declaration, so `List` can hold a pointer to it. `static int made;` declares a static data member, and the definition is outside the class. `static inline int open` is a declaration and a definition. `counter` and `twice` are declared before `main` and defined after it. `auto` deduces the return type of `greet` from its body.
