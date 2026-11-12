```cpp run id=containers
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Container {
    std::string name;
    bool refrigerated;
    bool clean;
};

class Spec {
public:
    explicit Spec(std::function<bool(const Container&)> test) : test_(std::move(test)) {}

    bool isSatisfiedBy(const Container& c) const { return test_(c); }

    Spec And(const Spec& other) const {
        Spec self = *this;
        return Spec([self, other](const Container& c) { return self.isSatisfiedBy(c) && other.isSatisfiedBy(c); });
    }
    Spec Or(const Spec& other) const {
        Spec self = *this;
        return Spec([self, other](const Container& c) { return self.isSatisfiedBy(c) || other.isSatisfiedBy(c); });
    }
    Spec Not() const {
        Spec self = *this;
        return Spec([self](const Container& c) { return !self.isSatisfiedBy(c); });
    }

private:
    std::function<bool(const Container&)> test_;
};

const std::vector<Container> containers = {{"c1", true, true}, {"c2", true, false}, {"c3", false, true}};

std::string select(const Spec& spec) {
    std::string out;
    for (const auto& c : containers) {
        if (!spec.isSatisfiedBy(c)) continue;
        if (!out.empty()) out += ", ";
        out += c.name;
    }
    return out;
}

int main() {
    Spec refrigerated([](const Container& c) { return c.refrigerated; });
    Spec clean([](const Container& c) { return c.clean; });

    std::cout << "meat: " << select(refrigerated.And(clean)) << "\n";
    std::cout << "not refrigerated: " << select(refrigerated.Not()) << "\n";
    std::cout << "refrigerated or clean: " << select(refrigerated.Or(clean)) << "\n";
}
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. `Spec` wraps a `std::function`, and each combined specification captures copies of its parts.
