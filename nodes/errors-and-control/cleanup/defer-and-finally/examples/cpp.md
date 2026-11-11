```cpp run id=copy
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class File {
public:
    explicit File(std::string name) : name_(std::move(name)) {
        std::cout << "open " << name_ << '\n';
    }
    ~File() { std::cout << "close " << name_ << '\n'; }
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class ScopeExit {
public:
    explicit ScopeExit(std::function<void()> f) : f_(std::move(f)) {}
    ~ScopeExit() { f_(); }
    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;

private:
    std::function<void()> f_;
};

void copy(const std::string& src, const std::string& dst, bool fail) {
    File a(src);
    File b(dst);
    if (fail) throw std::runtime_error("cannot read " + a.name());
    std::cout << "copy " << a.name() << " to " << b.name() << '\n';
}

int main() {
    copy("a.txt", "b.txt", false);
    try {
        copy("a.txt", "c.txt", true);
    } catch (const std::runtime_error& e) {
        std::cout << "caught: " << e.what() << '\n';
    }
    {
        ScopeExit log([] { std::cout << "scope exit runs last\n"; });
        std::cout << "work in the scope\n";
    }
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
open a.txt
open c.txt
close c.txt
close a.txt
caught: cannot read a.txt
work in the scope
scope exit runs last
```

- Verdict: native. C++ has no `finally`. A destructor runs when an object with automatic storage leaves its scope, on a normal exit and during stack unwinding.
- Objects are destroyed in reverse order of construction, so `b` closes before `a`.
- `ScopeExit` is a small class that runs a function in its destructor. It plays the role of `defer` for code that has no class of its own.

An object without a name is a temporary. The destructor of a temporary runs at the end of the full-expression that creates it, so this lock ends before the work that it should protect.

```cpp wrong id=temporary
#include <iostream>
#include <string>

class Lock {
public:
    explicit Lock(std::string name) : name_(std::move(name)) {
        std::cout << "lock " << name_ << '\n';
    }
    ~Lock() { std::cout << "unlock " << name_ << '\n'; }

private:
    std::string name_;
};

int main() {
    Lock{"accounts"};
    std::cout << "update accounts\n";
    Lock held{"orders"};
    std::cout << "update orders\n";
}
```

```output
lock accounts
unlock accounts
update accounts
lock orders
update orders
unlock orders
```
