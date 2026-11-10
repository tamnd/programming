```cpp run id=new-delete
#include <memory>
#include <print>
#include <string>

struct User {
    std::string name;
    explicit User(std::string n) : name(std::move(n)) { std::println("construct {}", name); }
    ~User() { std::println("destroy {}", name); }
};

int main() {
    User* admin = new User("admin");     // allocate, then construct
    std::println("hello, {}", admin->name);
    delete admin;                        // destroy, then deallocate

    int* totals = new int[3]{10, 20, 30};
    std::println("last total: {}", totals[2]);
    delete[] totals;                     // the array form matches new[]

    auto guest = std::make_unique<User>("guest");
    std::println("hello, {}", guest->name);
}                                        // unique_ptr deletes guest here
```

```output
construct admin
hello, admin
destroy admin
last total: 30
construct guest
hello, guest
destroy guest
```

- Verdict: native. A `new` expression allocates memory and runs a constructor, and a `delete` expression runs the destructor and frees the memory.
- `new[]` must be paired with `delete[]`, and `new` with `delete`. Any other pairing is undefined behaviour.
- `std::make_unique` makes an owner that calls `delete` in its destructor. The output shows `guest` destroyed at the end of `main` without an explicit `delete`.
- The default `operator new` throws `std::bad_alloc` on failure, so the program does not check for a null pointer.
