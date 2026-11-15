```cpp run id=record
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;

    bool operator==(const Person&) const = default;
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    return os << "Person{" << p.name << ", " << p.age << "}";
}

int main() {
    Person ada{.name = "Ada", .age = 36};
    std::cout << "name: " << ada.name << " age: " << ada.age << "\n";

    Person older = ada;  // copies every member, including the string
    older.age += 1;
    std::cout << "copy: " << older << "\n";
    std::cout << "original: " << ada << "\n";

    Person other{.name = "Ada", .age = 36};
    std::cout << "same fields equal: " << (ada == other ? "yes" : "no") << "\n";
}
```

```output for=record
name: Ada age: 36
copy: Person{Ada, 37}
original: Person{Ada, 36}
same fields equal: yes
```

- Verdict: native. A `struct` with public members is an aggregate, a record that brace initialization fills.
- A defaulted `operator==` (C++20) compares the members in order.
- Copying a `Person` copies the `std::string` too, so the copy shares nothing with the original.
