```cpp run id=repository
#include <iostream>
#include <map>
#include <memory>
#include <string>

class Repository {
public:
    virtual ~Repository() = default;
    virtual std::string get(const std::string& key) = 0;
    virtual std::string name() const = 0;
};

class Store : public Repository {
public:
    std::string get(const std::string& key) override {
        std::cout << "store: load " << key << "\n";
        return "value-" + key;
    }
    std::string name() const override { return "store"; }
};

class Logged : public Repository {
public:
    explicit Logged(std::unique_ptr<Repository> inner) : inner_(std::move(inner)) {}
    std::string get(const std::string& key) override {
        std::cout << "log: get " << key << "\n";
        return inner_->get(key);
    }
    std::string name() const override { return inner_->name(); }

private:
    std::unique_ptr<Repository> inner_;
};

class Cached : public Repository {
public:
    explicit Cached(std::unique_ptr<Repository> inner) : inner_(std::move(inner)) {}
    std::string get(const std::string& key) override {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;
        }
        std::string value = inner_->get(key);
        cache_.emplace(key, value);
        return value;
    }
    std::string name() const override { return inner_->name(); }

private:
    std::unique_ptr<Repository> inner_;
    std::map<std::string, std::string> cache_;
};

int main() {
    std::unique_ptr<Repository> repo =
        std::make_unique<Logged>(std::make_unique<Cached>(std::make_unique<Store>()));
    for (const char* key : {"a", "a", "b"}) {
        std::cout << repo->get(key) << "\n";
    }
    std::cout << "name: " << repo->name() << "\n";
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. The decorators derive from the abstract class `Repository` and own the inner object through `std::unique_ptr`. When the outer decorator is destroyed, the whole chain is destroyed.
- The virtual destructor makes it safe to destroy a `Store` through a pointer to `Repository`.
- The chain can be known at compile time. Then a template `Logged<Inner>` that holds `Inner` by value removes the virtual calls, as in the Rust example.
