```cpp run id=car
#include <iostream>
#include <memory>
#include <string>

struct StarterPart {
    virtual ~StarterPart() = default;
    virtual std::string crank() const = 0;
};

struct Starter : StarterPart {
    std::string crank() const override { return "crank the engine"; }
};

struct NewStarter : StarterPart {
    std::string crank() const override { return "crank the engine with the new starter"; }
};

class Car {
public:
    std::string drive() const {
        return starter_->crank() + ", " + engine_.fire() + ", " + wheels_.turn();
    }
    void replace_starter(std::unique_ptr<StarterPart> starter) { starter_ = std::move(starter); }

private:
    struct Engine {
        std::string fire() const { return "fire the cylinders"; }
    };
    struct Wheels {
        std::string turn() const { return "turn the wheels"; }
    };
    // The car owns its parts: they are destroyed with it.
    std::unique_ptr<StarterPart> starter_ = std::make_unique<Starter>();
    Engine engine_;
    Wheels wheels_;
};

int main() {
    Car car;
    std::cout << "drive: " << car.drive() << "\n";
    car.replace_starter(std::make_unique<NewStarter>());
    std::cout << "drive: " << car.drive() << "\n";
}
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. The parts are private members, so their lifetime is the lifetime of the whole, and `unique_ptr` gives the whole sole ownership of the part that can be replaced.
