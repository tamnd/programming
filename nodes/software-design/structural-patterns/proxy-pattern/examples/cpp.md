```cpp run id=report
#include <functional>
#include <iostream>
#include <memory>
#include <string>

struct Report {
    virtual ~Report() = default;
    virtual std::string render() const = 0;
};

struct DiskReport final : Report {
    DiskReport() { std::cout << "load: read the report from disk\n"; }
    std::string render() const override { return "report: " + std::to_string(orders) + " orders"; }
    int orders = 42;
};

// A smart reference: operator-> makes the object on first use and then forwards to it.
template <class T>
class LazyPtr {
public:
    explicit LazyPtr(std::function<std::unique_ptr<T>()> factory) : factory_(std::move(factory)) {}
    T* operator->() {
        if (!real_) {
            real_ = factory_();
        }
        return real_.get();
    }

private:
    std::function<std::unique_ptr<T>()> factory_;
    std::unique_ptr<T> real_;
};

// A virtual proxy with the same interface as the real report.
class LazyReport final : public Report {
public:
    LazyReport() : real_([] { return std::make_unique<DiskReport>(); }) {}
    std::string render() const override { return real_->render(); }

private:
    mutable LazyPtr<DiskReport> real_;  // mutable: loading does not change what render returns
};

// A protection proxy.
class GuardedReport final : public Report {
public:
    GuardedReport(const Report& inner, std::string role) : inner_(inner), role_(std::move(role)) {}
    std::string render() const override { return role_ == "admin" ? inner_.render() : "access denied"; }

private:
    const Report& inner_;
    std::string role_;
};

int main() {
    LazyReport report;
    std::cout << "proxy made\n";
    std::cout << report.render() << "\n";
    std::cout << report.render() << "\n";
    for (const char* role : {"guest", "admin"}) {
        std::cout << role << ": " << GuardedReport(report, role).render() << "\n";
    }
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: idiom. Design Patterns shows a smart reference in C++ that overloads `operator->`. `LazyPtr` is one: `real_->render()` calls `operator->`, which makes the object if it does not exist, and the call goes on to the pointer it returns.
- `std::unique_ptr` and `std::shared_ptr` are proxies of the same kind. They forward `->` and `*` to the object, and they add ownership.
- `render` is `const`, but the first call fills the pointer. The member is `mutable`, because loading does not change the value that the report shows. A proxy shared by threads needs `std::call_once` or a mutex here.
- `GuardedReport` holds a reference, so it must not outlive the report that it guards.
