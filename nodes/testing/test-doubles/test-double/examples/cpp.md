```cpp run id=register
#include <print>
#include <set>
#include <string>
#include <utility>
#include <vector>

// A seam at run time: an abstract class with virtual functions.
struct Users {
    virtual ~Users() = default;
    virtual bool exists(const std::string& email) const = 0;
    virtual void add(const std::string& email) = 0;
};

struct FakeUsers : Users {
    std::set<std::string> emails;
    bool exists(const std::string& email) const override { return emails.contains(email); }
    void add(const std::string& email) override { emails.insert(email); }
};

// A seam at compile time: any type with send(to, body) can be the mailer.
template <typename Mailer>
std::string register_user(const std::string& email, Users& users, Mailer& mailer) {
    if (users.exists(email)) {
        return "taken";
    }
    users.add(email);
    mailer.send(email, "Welcome!");
    return "created";
}

struct SpyMailer {
    std::vector<std::pair<std::string, std::string>> sent;
    void send(const std::string& to, const std::string& body) { sent.emplace_back(to, body); }
};

int main() {
    FakeUsers users;
    SpyMailer mailer;
    std::println("{}", register_user("ada@example.com", users, mailer));
    std::println("{}", register_user("ada@example.com", users, mailer));
    for (const auto& [to, body] : mailer.sent) {
        std::println("sent {} to {}", body, to);
    }
}
```

```output
created
taken
sent Welcome! to ada@example.com
```

- Verdict: idiom. The C++ standard library has no mock facility. The language gives two seams.
- The store is a seam at run time. `Users` is an abstract class with virtual functions, and `FakeUsers` overrides them. Each call goes through the vtable.
- The mailer is a seam at compile time. `register_user` is a template, so any type with a member `send(to, body)` works, and the call needs no vtable. The cost is that the template must be visible in a header.
- A function that is not virtual and not a template cannot be replaced by a double without a change to the code or to the link step.
