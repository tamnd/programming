```cpp run id=move
#include <iostream>
#include <string>
#include <vector>

struct Position {
    int x, y;
};

class Movable {
public:
    virtual ~Movable() = default;
    virtual Position getPosition() const = 0;
    virtual void setPosition(Position p) = 0;
};

class MoveServant {
public:
    void moveTo(Movable& serviced, Position where) const { serviced.setPosition(where); }
    void moveBy(Movable& serviced, int dx, int dy) const {
        Position p = serviced.getPosition();
        serviced.setPosition({p.x + dx, p.y + dy});
    }
};

class Shape : public Movable {
public:
    Shape(std::string name, Position p) : name_(std::move(name)), p_(p) {}
    Position getPosition() const override { return p_; }
    void setPosition(Position p) override { p_ = p; }
    std::string str() const {
        return name_ + " at (" + std::to_string(p_.x) + ", " + std::to_string(p_.y) + ")";
    }

private:
    std::string name_;
    Position p_;
};

struct Rectangle : Shape { explicit Rectangle(Position p) : Shape("rectangle", p) {} };
struct Ellipse : Shape { explicit Ellipse(Position p) : Shape("ellipse", p) {} };
struct Triangle : Shape { explicit Triangle(Position p) : Shape("triangle", p) {} };

int main() {
    MoveServant servant;
    Rectangle r({0, 0});
    Ellipse e({10, 0});
    Triangle t({0, 10});
    std::vector<Shape*> shapes = {&r, &e, &t};
    for (Shape* s : shapes) {
        servant.moveBy(*s, 3, 4);
        std::cout << s->str() << "\n";
    }
    servant.moveTo(t, {1, 1});
    std::cout << t.str() << "\n";
}
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. An abstract class `Movable` states what the servant needs.
