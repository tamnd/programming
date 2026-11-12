```cpp run id=collisions
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Ship;
struct Asteroid;

struct Body {
    virtual ~Body() = default;
    virtual std::string name() const = 0;
    virtual std::string collide(const Body& other) const = 0;
    virtual std::string collideWith(const Ship& ship) const = 0;
    virtual std::string collideWith(const Asteroid& asteroid) const = 0;
};

struct Ship : Body {
    std::string name() const override { return "ship"; }
    // *this has the static type Ship here, so the Ship overload is chosen
    std::string collide(const Body& other) const override { return other.collideWith(*this); }
    std::string collideWith(const Ship&) const override { return "ships bounce"; }
    std::string collideWith(const Asteroid&) const override { return "ship takes damage"; }
};

struct Asteroid : Body {
    std::string name() const override { return "asteroid"; }
    std::string collide(const Body& other) const override { return other.collideWith(*this); }
    std::string collideWith(const Ship&) const override { return "ship takes damage"; }
    std::string collideWith(const Asteroid&) const override { return "asteroids break apart"; }
};

int main() {
    std::vector<std::pair<std::unique_ptr<Body>, std::unique_ptr<Body>>> pairs;
    pairs.emplace_back(std::make_unique<Ship>(), std::make_unique<Asteroid>());
    pairs.emplace_back(std::make_unique<Asteroid>(), std::make_unique<Ship>());
    pairs.emplace_back(std::make_unique<Asteroid>(), std::make_unique<Asteroid>());
    pairs.emplace_back(std::make_unique<Ship>(), std::make_unique<Ship>());
    for (const auto& [a, b] : pairs) {
        std::cout << a->name() << " + " << b->name() << ": " << a->collide(*b) << "\n";
    }
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. The callbacks are overloads of `collideWith`. Inside each class the static type of `*this` is that class, so overload resolution picks the correct callback, and the virtual call picks the class of the argument.
