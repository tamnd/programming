```cpp run id=spawner
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Prototype {
public:
    virtual ~Prototype() = default;
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual std::string describe() const = 0;

protected:
    Prototype() = default;
    Prototype(const Prototype&) = default;  // protected, so no caller can slice a copy
    Prototype& operator=(const Prototype&) = default;
};

class Monster : public Prototype {
public:
    Monster(std::string name, int health, std::vector<std::string> items)
        : name_(std::move(name)), health_(health), items_(std::move(items)) {}

    // The copy constructor that the compiler writes copies the vector and its strings.
    std::unique_ptr<Prototype> clone() const override { return std::make_unique<Monster>(*this); }

    std::string describe() const override {
        std::string text = name_ + ": health " + std::to_string(health_) + ", items ";
        for (std::size_t i = 0; i < items_.size(); ++i) {
            text += (i > 0 ? ", " : "") + items_[i];
        }
        return text;
    }

    void add_item(std::string item) { items_.push_back(std::move(item)); }

private:
    std::string name_;
    int health_;
    std::vector<std::string> items_;
};

class Trap : public Prototype {
public:
    Trap(std::string name, int damage) : name_(std::move(name)), damage_(damage) {}

    std::unique_ptr<Prototype> clone() const override { return std::make_unique<Trap>(*this); }

    std::string describe() const override { return name_ + ": damage " + std::to_string(damage_); }

private:
    std::string name_;
    int damage_;
};

int main() {
    std::map<std::string, std::unique_ptr<Prototype>> registry;
    registry["goblin"] = std::make_unique<Monster>("goblin", 30, std::vector<std::string>{"dagger"});
    registry["spikes"] = std::make_unique<Trap>("spikes", 5);
    auto spawn = [&](const std::string& key) { return registry.at(key)->clone(); };

    auto first = spawn("goblin");
    dynamic_cast<Monster&>(*first).add_item("torch");
    auto second = spawn("goblin");
    auto trap = spawn("spikes");
    for (const auto* thing : {first.get(), second.get(), trap.get()}) {
        std::cout << thing->describe() << '\n';
    }
    std::cout << "prototype " << registry.at("goblin")->describe() << '\n';
}
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
```

- Verdict: idiom. A virtual `clone` that returns `std::unique_ptr` to the base class. Each class implements it with its own copy constructor, so the copy has the most derived type.
- A `std::vector` and a `std::string` are values: the copy constructor copies their elements. A raw pointer member is copied as an address, so a class with one needs its own copy constructor.
- The base class makes its copy constructor `protected`. A public one would let a caller copy a `Monster` into a `Prototype` by value and lose the derived part, which is slicing. The C++ Core Guidelines give this rule and the virtual `clone` as the fix.
