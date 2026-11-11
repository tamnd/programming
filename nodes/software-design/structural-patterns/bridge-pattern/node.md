---
tag: GA90WF
kind: pattern
slug: bridge-pattern
name: Bridge pattern
aliases: [Bridge, Handle/Body, pimpl, pointer to implementation]
area: software-design/structural-patterns
level: core
summary: >-
  Split a class that varies in two ways into two class hierarchies, an abstraction and an implementation, and join them with a reference. Each hierarchy then grows without the other.
family: structural-design
polarity: positive
intent: "Separate an abstraction from its implementation, so that each can change and grow without the other."
also_known_as: [Handle/Body]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with a window abstraction that must run on more than one window system as the motivating example.
see_also: [adapter-pattern, strategy-pattern, abstract-factory-pattern, state-pattern, proxy-pattern, decorator-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Bridge (Intent, Also Known As, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#i27-for-stable-library-abi-consider-the-pimpl-idiom
    title: C++ Core Guidelines
    locator: I.27, For stable library ABI, consider the Pimpl idiom (Reason, Example)
  - url: https://pkg.go.dev/log/slog
    title: Go standard library, package slog
    locator: package overview; type Handler; Writing a handler
  - url: https://pkg.go.dev/database/sql
    title: Go standard library, package sql
    locator: package overview; type DB
  - url: https://pkg.go.dev/database/sql/driver
    title: Go standard library, package driver
    locator: package overview
  - url: https://pkg.go.dev/go/build#hdr-Build_Constraints
    title: Go standard library, package build
    locator: Build Constraints
---

## Intent

Separate an abstraction from its implementation, so that each can change and grow without the other.[^1] The abstraction is the interface that clients use. The implementation is a second interface with primitive operations. The abstraction holds a reference to an implementation object and calls it.

## Problem

Design Patterns starts with a window abstraction for a user interface toolkit. The toolkit must run on more than one window system. A direct design is a subclass of the window for each window system. When the toolkit adds a kind of window, such as an icon window, it needs a subclass of that kind for each window system as well.[^1]

The same shape appears in many programs. A drawing program has shapes, such as circles and squares, and output formats, such as plain text and SVG. With inheritance alone, each pair is a class:

```text
Shape
 |-- TextCircle    |-- SvgCircle
 |-- TextSquare    |-- SvgSquare
```

Two shapes and two formats need four classes. Three shapes and three formats need nine. A fourth format adds three classes, one for each shape. The code that writes SVG is in two or three classes, and the code that computes the corners of a square is in each format's square. The count is the product of the two dimensions, and each class is bound to one format at compile time.

The forces are these. The program has two dimensions that change for different reasons and at different times. The client code wants to use a shape and not care how it is drawn. And the program may want to choose the format at run time, or share one format object between many shapes.

## Solution

Split the one hierarchy into two, and join them with a reference. Design Patterns names four participants.[^1]

- **Abstraction** defines the interface for clients. It holds a reference to an Implementor. Here it is `Shape`.
- **RefinedAbstraction** extends the Abstraction. Here it is `Circle` and `Square`.
- **Implementor** defines the interface of the implementation classes. Its operations are usually primitive. The Abstraction builds its higher-level operations from them. Here it is `Renderer`, with `circle` and `polygon`.
- **ConcreteImplementor** implements the Implementor for one platform. Here it is `TextRenderer` and `SvgRenderer`.

```text
        Abstraction                               Implementor
   Shape: renderer, draw(), scale()  ------>  Renderer: circle(), polygon()
        ^            ^                             ^              ^
        |            |                             |              |
     Circle        Square                   TextRenderer      SvgRenderer
   draw() calls   draw() computes 4 corners
   circle()       and calls polygon()
```

Two shapes and two renderers are four classes. A third renderer adds one class. A third shape adds one class. The count is now a sum and not a product.

The two interfaces do not have to match. The Implementor offers what every platform can do. The Abstraction offers what clients want. `Square.draw` turns a square into four points and calls `polygon`. No renderer knows what a square is, and no shape knows what SVG is.

### The abstraction must not know the concrete implementors

The bridge works only when the abstraction calls the Implementor interface and nothing else. A test of the concrete class in the abstraction joins the two hierarchies again. Each renderer added later then needs a change in each shape. When the change is forgotten, the result is wrong without an error:

```python wrong id=type-test
class TextRenderer:
    pass


class SvgRenderer:
    pass


class CanvasRenderer:  # added later; Circle.draw was not changed
    pass


class Circle:
    def __init__(self, renderer, radius):
        self.renderer = renderer
        self.radius = radius

    def draw(self):
        # the abstraction picks its code by the class of the renderer
        if isinstance(self.renderer, TextRenderer):
            return f"circle radius {self.radius}"
        if isinstance(self.renderer, SvgRenderer):
            return f'<circle r="{self.radius}"/>'
        return ""


for renderer in (TextRenderer(), SvgRenderer(), CanvasRenderer()):
    print(f"{type(renderer).__name__}: {Circle(renderer, 2).draw()!r}")
```

```output for=type-test
TextRenderer: 'circle radius 2'
SvgRenderer: '<circle r="2"/>'
CanvasRenderer: ''
```

The canvas renderer draws nothing, and nothing reports it. The fix moves the format code into the renderers, behind a `circle` method that each renderer must define, as the examples do.

### The degenerate case: one implementor and Pimpl

Design Patterns says that the pattern is useful even with one ConcreteImplementor.[^1] The split then has another purpose: a change in the implementation must not force a change in the client code. In C++, the private members of a class are part of its layout, so a change to them makes each client compile again. The C++ Core Guidelines describe the Pimpl idiom, a "pointer to implementation". The class in the header holds a `std::unique_ptr` to a class that only the source file defines. This isolates clients from changes in the implementation, at the cost of an indirection.[^2]

```cpp run id=pimpl
#include <iostream>
#include <memory>
#include <string>

// widget.h: the part that clients include
class Widget {
public:
    explicit Widget(std::string title);
    ~Widget();  // defined below, where Impl is a complete type
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;
    void rename(std::string title);
    std::string describe() const;

private:
    struct Impl;  // declared here, defined in widget.cpp
    std::unique_ptr<Impl> impl_;
};

// widget.cpp: a change here does not change widget.h
struct Widget::Impl {
    std::string title;
    int revisions = 0;
};

Widget::Widget(std::string title) : impl_(std::make_unique<Impl>()) { impl_->title = std::move(title); }
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::rename(std::string title) {
    impl_->title = std::move(title);
    ++impl_->revisions;
}

std::string Widget::describe() const {
    return impl_->title + " (revisions: " + std::to_string(impl_->revisions) + ")";
}

int main() {
    Widget widget("draft");
    widget.rename("report");
    std::cout << widget.describe() << '\n';
}
```

```output for=pimpl
report (revisions: 1)
```

The destructor and the move operations are declared in the class and defined after `Impl`. The `std::unique_ptr` must delete an `Impl`, and that needs the complete type. The Core Guidelines example has the same comment on its destructor.[^2] Design Patterns lists Handle/Body as another name for the pattern.[^1] The handle is the class that clients see, and the body is the implementation behind it.

## Example

Each example file builds two shapes, a circle and a square, and two renderers, text and SVG. For each renderer, the program draws each shape, scales it by 2 and draws it again. Then it changes the renderer of one circle at run time.

| Language | Abstraction | Implementor |
|---|---|---|
| Python | `abc.ABC` subclasses with a `renderer` attribute | `abc.ABC` subclasses |
| JavaScript | classes with a `renderer` field | classes with the same method names |
| TypeScript | an `abstract class` | an `interface`, implemented by object literals |
| Java | an `abstract class` with a `protected` field | an `interface` |
| C# | an `abstract class` with a primary constructor | an `interface` |
| Go | an interface and structs with a `Renderer` field | an interface |
| Rust | a trait and structs with an `Rc<dyn Renderer>` field | a trait |
| C++ | an abstract class with a `std::shared_ptr<const Renderer>` | an abstract class |

## Consequences

Design Patterns lists three consequences.[^1]

- **The interface and the implementation are decoupled.** The implementation is not bound to the abstraction for good. A program can choose it, or change it, at run time. A change to an implementation class does not force a change to the abstraction and its clients.
- **Extensibility improves.** The Abstraction and Implementor hierarchies grow on their own.
- **Clients do not see the implementation details.** Design Patterns gives the sharing of implementor objects as an example.

The costs are these. Each call from the abstraction to the implementor is an indirect call through a reference. The design has two hierarchies and an extra interface, so a reader must follow the reference to see what a call does. And the Implementor interface is hard to change. Each primitive operation must work for each implementor, and a primitive that one platform cannot do well is a problem for all of them. The split is a guess about which way the code will vary. When the guess is wrong, the cost of the extra interface buys nothing.

## When not to use it

Do not use it when there is one dimension of change. A class with several implementations is an interface with several classes, or a [[strategy-pattern]]. Bridge is for the case where the abstraction itself also has subclasses.

Do not use it when one of the two dimensions has one member and will not grow, unless the purpose is the compile-time isolation of Pimpl. One renderer behind a `Renderer` interface is one more file and one more indirection.

Do not use it to join two interfaces that already exist. That is [[adapter-pattern]].

Do not build it for the platform choice when the language or the build does that already. Go includes a file in a package only when its build constraints hold, and a name such as `source_windows.go` is a constraint on the target system.[^6] C and C++ select code with the preprocessor. These choose an implementation at build time with no interface and no reference. A run-time bridge is needed when the choice is made at run time, or when one program uses more than one implementation.

## Variants and related patterns

**Adapter.** Both patterns forward calls from one interface to another object. Design Patterns draws the difference by time. An adapter makes two classes work together after they are designed. A bridge is designed up front, so that the abstraction and the implementation can vary on their own.[^1] An adapter changes an interface that exists. A bridge defines both interfaces for this purpose. See [[adapter-pattern]].

**Strategy.** The class diagrams of Bridge and Strategy look the same: an object holds a reference to an interface, and calls it. The difference is in the purpose and in the two sides. A strategy is an algorithm that a client chooses for a context. The context is usually one class, and the strategy interface is often one method. A bridge splits a class hierarchy in two. The abstraction has its own subclasses, and the implementor interface is a set of primitive operations from which the abstraction builds its own operations. Strategy is a behavioural pattern about a choice of algorithm. Bridge is a structural pattern about the shape of two hierarchies. See [[strategy-pattern]].

**Abstract Factory.** Design Patterns says that an abstract factory can create and configure a particular bridge.[^1] When a program must pick a set of implementors that work together, such as the windows, fonts and colours of one window system, a factory for that platform makes them. See [[abstract-factory-pattern]].

**State.** A state object is also an implementation behind a reference. The context replaces it when its state changes, and the client does not choose it. See [[state-pattern]].

**Proxy and Decorator.** These also hold a reference to another object, but they have the same interface as that object. A bridge's abstraction and implementor have different interfaces. See [[proxy-pattern]] and [[decorator-pattern]].

**Static bridge.** A generic or template parameter can take the place of the reference: `Circle<R: Renderer>` in Rust, `template <class R> class Circle` in C++. The compiler makes one class for each implementor, and calls need no dynamic dispatch. The price is that the implementor is fixed at compile time and is part of the type.

## Known uses

- Go `log/slog`: each `Logger` is associated with a `Handler`. A `Logger` output method creates a `Record` from its arguments and passes it to the `Handler`, which decides how to handle it.[^3] The `Logger` has the convenience methods, and each `Handler` implements a small interface.
- Go `database/sql`: the package must be used together with a database driver.[^4] Package `database/sql/driver` defines the interfaces that the drivers implement.[^5] `sql.DB`, which manages a pool of connections, is the abstraction, and each driver is an implementor.
- C++ libraries that keep a stable ABI use the Pimpl idiom, which the C++ Core Guidelines recommend for this purpose.[^2]

## References
