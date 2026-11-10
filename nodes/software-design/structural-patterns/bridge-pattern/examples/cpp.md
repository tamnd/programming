```cpp run id=shapes
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Point {
    int x, y;
};

// The implementor: the primitive drawing operations of one output format.
class Renderer {
public:
    virtual ~Renderer() = default;
    virtual std::string name() const = 0;
    virtual std::string circle(int x, int y, int radius) const = 0;
    virtual std::string polygon(const std::vector<Point>& points) const = 0;
};

class TextRenderer final : public Renderer {
public:
    std::string name() const override { return "text"; }
    std::string circle(int x, int y, int radius) const override {
        return "circle at (" + std::to_string(x) + ", " + std::to_string(y) + ") radius " +
               std::to_string(radius);
    }
    std::string polygon(const std::vector<Point>& points) const override {
        std::string out = "polygon";
        for (const auto& p : points) {
            out += " (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
        }
        return out;
    }
};

class SvgRenderer final : public Renderer {
public:
    std::string name() const override { return "svg"; }
    std::string circle(int x, int y, int radius) const override {
        return "<circle cx=\"" + std::to_string(x) + "\" cy=\"" + std::to_string(y) + "\" r=\"" +
               std::to_string(radius) + "\"/>";
    }
    std::string polygon(const std::vector<Point>& points) const override {
        std::string out;
        for (const auto& p : points) {
            out += (out.empty() ? "" : " ") + std::to_string(p.x) + "," + std::to_string(p.y);
        }
        return "<polygon points=\"" + out + "\"/>";
    }
};

// The abstraction: it holds a renderer and draws itself with the primitives.
class Shape {
public:
    explicit Shape(std::shared_ptr<const Renderer> renderer) : renderer_(std::move(renderer)) {}
    virtual ~Shape() = default;
    void set_renderer(std::shared_ptr<const Renderer> renderer) { renderer_ = std::move(renderer); }
    virtual std::string draw() const = 0;
    virtual void scale(int factor) = 0;

protected:
    std::shared_ptr<const Renderer> renderer_;
};

class Circle final : public Shape {
public:
    Circle(std::shared_ptr<const Renderer> renderer, int x, int y, int radius)
        : Shape(std::move(renderer)), x_(x), y_(y), radius_(radius) {}
    std::string draw() const override { return renderer_->circle(x_, y_, radius_); }
    void scale(int factor) override { radius_ *= factor; }

private:
    int x_, y_, radius_;
};

class Square final : public Shape {
public:
    Square(std::shared_ptr<const Renderer> renderer, int x, int y, int side)
        : Shape(std::move(renderer)), x_(x), y_(y), side_(side) {}
    std::string draw() const override {
        return renderer_->polygon({{x_, y_}, {x_ + side_, y_}, {x_ + side_, y_ + side_}, {x_, y_ + side_}});
    }
    void scale(int factor) override { side_ *= factor; }

private:
    int x_, y_, side_;
};

int main() {
    std::vector<std::shared_ptr<const Renderer>> renderers{
        std::make_shared<TextRenderer>(), std::make_shared<SvgRenderer>()};
    for (const auto& renderer : renderers) {
        std::cout << renderer->name() << ":\n";
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Circle>(renderer, 1, 1, 2));
        shapes.push_back(std::make_unique<Square>(renderer, 0, 0, 3));
        for (auto& shape : shapes) {
            std::cout << "  " << shape->draw() << '\n';
            shape->scale(2);
            std::cout << "  " << shape->draw() << '\n';
        }
    }

    Circle circle(std::make_shared<TextRenderer>(), 1, 1, 2);
    circle.set_renderer(std::make_shared<SvgRenderer>());  // the implementation changes at run time
    std::cout << "switched: " << circle.draw() << '\n';
}
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. Two abstract classes with pure virtual functions. The abstraction holds a `std::shared_ptr<const Renderer>`, so many shapes can share one renderer.
- The Pimpl idiom is the degenerate case with one implementor. A class holds a `std::unique_ptr` to an incomplete type that its source file defines. The header then does not change when the implementation changes. The C++ Core Guidelines suggest it for a stable library ABI. The node page has an example.
- A template parameter, `template <class R> class Circle`, gives the static form: no virtual calls, but the renderer is fixed at compile time.
