```cpp run id=breeds
#include <iostream>
#include <string>
#include <vector>

class Monster;

class Breed {
public:
    Breed(std::string name, int health, std::string attack)
        : name_(std::move(name)), health_(health), attack_(std::move(attack)) {}
    Monster newMonster() const;
    const std::string& name() const { return name_; }
    int health() const { return health_; }
    const std::string& attack() const { return attack_; }

private:
    std::string name_;
    int health_;
    std::string attack_;
};

class Monster {
public:
    explicit Monster(const Breed& breed) : breed_(breed), health_(breed.health()) {}
    std::string describe() const {
        return breed_.name() + ": " + std::to_string(health_) +
               " health, attacks with " + breed_.attack();
    }

private:
    const Breed& breed_;  // the breed must live longer than the monster
    int health_;          // each monster has its own health
};

Monster Breed::newMonster() const { return Monster(*this); }

int main() {
    // New kinds are data, for example from a file.
    const std::vector<Breed> breeds = {
        {"goblin", 20, "a dagger"},
        {"dragon", 200, "fire"},
        {"wyvern", 120, "its tail"},
    };
    for (const Breed& b : breeds) {
        std::cout << b.newMonster().describe() << "\n";
    }
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A class with one instance for each kind. The program must keep each breed alive as long as its monsters.
