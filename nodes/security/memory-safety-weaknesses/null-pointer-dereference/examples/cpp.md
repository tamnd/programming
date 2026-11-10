```cpp fails expect=sanitizer:undefined timeout=30 id=raw-pointer
#include <iostream>
#include <map>
#include <string>

struct User {
    int quota;
};

const std::map<std::string, User> users = {{"alice", {10}}, {"bob", {20}}};

// Returns the user, or nullptr if there is none.
const User* find_user(const std::string& name) {
    auto it = users.find(name);
    return it == users.end() ? nullptr : &it->second;
}

int main() {
    for (std::string name : {"alice", "nobody"}) {
        const User* u = find_user(name);
        std::cout << name << ": quota " << u->quota << "\n"; // u can be nullptr
    }
}
```

UndefinedBehaviorSanitizer reports a `member access within null pointer of type 'const User'`. The raw pointer has the same two meanings as in C.

```cpp run id=optional
#include <iostream>
#include <map>
#include <optional>
#include <string>

struct User {
    int quota;
};

const std::map<std::string, User> users = {{"alice", {10}}, {"bob", {20}}};

// The return type says that the user can be absent.
std::optional<User> find_user(const std::string& name) {
    auto it = users.find(name);
    if (it == users.end()) {
        return std::nullopt;
    }
    return it->second;
}

int main() {
    for (std::string name : {"alice", "nobody"}) {
        if (auto u = find_user(name)) {
            std::cout << name << ": quota " << u->quota << "\n";
        } else {
            std::cout << name << ": no such user\n";
        }
    }
}
```

```output for=optional
alice: quota 10
nobody: no such user
```

- Verdict: stdlib. `std::optional` puts absence in the type, and a reference cannot be null. Raw pointers and smart pointers can still be null.
- `*opt` and `opt->` on an empty `std::optional` are undefined behavior. Test the optional first, or call `value()`, which throws `std::bad_optional_access`.
- Return a reference when a value is always present, and an optional when it can be absent.
