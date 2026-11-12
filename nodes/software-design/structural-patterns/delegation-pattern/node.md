---
tag: FHMA2X
kind: pattern
slug: delegation-pattern
name: Delegation pattern
aliases: [Delegation, delegate, delegation pattern, forwarding]
area: software-design/structural-patterns
level: core
summary: >-
  An object hands a request to a helper object, its delegate, and passes itself when the delegate needs it. This gives the reuse of inheritance through composition.
family: structural-design
polarity: positive
intent: "Let an object reuse the behaviour of another object by handing requests to it, in place of inheriting from its class, so that composition is as powerful for reuse as inheritance."
also_known_as: [Delegate]
history:
  year: 1986
  people: [Henry Lieberman]
  note: Lieberman defined delegation as a language feature in his 1986 paper on prototypical objects. Design Patterns (1994) described the explicit form, where the receiver passes itself to the delegate, and used it in State, Strategy and Visitor.
see_also: [composition-over-inheritance, class-inheritance, prototype-based-programming, decorator-pattern, proxy-pattern, strategy-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Delegation_pattern
    title: Delegation pattern, Wikipedia
    locator: Introduction; Definition; Language support
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 1, Introduction, Delegation (pages 20 and 21)
  - url: https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)
    title: Delegation (object-oriented programming), Wikipedia
    locator: Introduction; Overview; Language support for delegation
  - url: https://kotlinlang.org/docs/delegation.html
    title: Delegation, Kotlin Documentation
    locator: Delegation; Overriding a member of an interface implemented by delegation
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types (embedded fields and promoted methods)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
    title: Inheritance and the prototype chain, MDN Web Docs
    locator: Introduction; Inheriting "methods"
  - url: https://docs.python.org/3.14/reference/datamodel.html#object.__getattr__
    title: The Python Language Reference, Data model, Python 3.14
    locator: object.__getattr__
  - url: https://doc.rust-lang.org/std/ops/trait.Deref.html
    title: Deref, The Rust Standard Library
    locator: When to implement Deref or DerefMut
  - url: https://github.com/golang/go/blob/go1.25.0/src/bufio/bufio.go#L835-L845
    title: Go go1.25.0 source, src/bufio/bufio.go
    locator: ReadWriter and NewReadWriter, lines 835 to 845
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/tempfile.py#L487-L527
    title: CPython v3.14.0 source, Lib/tempfile.py
    locator: _TemporaryFileWrapper and its __getattr__, lines 487 to 527
---

## Intent

Let an object reuse the behaviour of another object by handing requests to it, in place of inheriting from its class. Two objects handle each request: the receiving object, and its **delegate**.[^1][^2]

## Problem

A `Window` needs an area. A `Rectangle` class already computes one. If `Window` inherits from `Rectangle`, every window is a rectangle for all of its life, and it gets every public method of `Rectangle`, also those that make no sense for a window. A round window then needs a second subclass, and a window that changes its shape cannot exist.

The forces are these:

- The new class needs some behaviour of an existing class, but it is not a kind of that class.
- The behaviour can change for one object, or at run time.
- The reused code sometimes needs to call back to the object that uses it.

## Solution

Give the receiving object a field that holds the delegate. For each request that the delegate handles, the receiver calls the matching method of the delegate. When the delegate needs the receiver, the receiver passes itself as an argument.[^2]

```text
 Client            Window (receiver)                 Rectangle (delegate)
 ------            -----------------                 --------------------
 w.area() -------> shape.area() -------------------> width * height
 w.describe() ---> shape.describe(self) -----------> uses owner.title
                   field: shape  (can be replaced at run time)
```

Design Patterns describes delegation as a way to make composition as powerful for reuse as inheritance. With inheritance, an inherited operation can always refer to the receiving object through `this` or `self`. With delegation, the receiver passes itself to the delegate to get the same effect.[^2][^1]

**Delegation and forwarding.** The word "delegation" is often used for plain forwarding, where the receiver calls the delegate and the call runs in the context of the delegate only.[^1] In the strict sense, delegation evaluates a member of one object in the context of the original sender.[^3] Lieberman defined this sense in 1986 as a language feature, and Self and JavaScript support it in the language.[^3] A language without that support must pass the sender by hand.

Some languages generate the forwarding code:

| Language | Support |
|---|---|
| Kotlin | `class Derived(b: Base) : Base by b` generates every member of `Base` and forwards it to `b`[^4] |
| Go | an embedded field promotes its methods to the outer struct[^5] |
| JavaScript | a method found on the prototype runs with `this` set to the inheriting object[^6] |
| Python | `__getattr__` runs when normal lookup fails, so it can forward every missing attribute[^7] |
| Rust | no forwarding keyword; `Deref` is meant for smart pointers, not for general delegation[^8] |

## Example

The example files show one program in each language. A `Window` delegates `area` to a shape, and it delegates `describe` to the shape with itself as the argument, so that the shape can read the title of the window. Then the program replaces the rectangle with a triangle, and the same window changes its behaviour:

```text
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

Each example uses an interface or a protocol for the shape, so that `Window` does not depend on one class. The Go example embeds the `Shape` interface in the struct, so `Area` is a promoted method and needs no forwarding method.

In JavaScript, the prototype chain gives delegation with no code. The method is found on the prototype, but `this` is the object that received the call:[^6]

```javascript run id=prototype-delegation
const rectangle = {
  describe() {
    return `${this.title}: a rectangle`;
  },
};

const window1 = Object.create(rectangle);
window1.title = "main";
console.log(window1.describe());
console.log(Object.hasOwn(window1, "describe"));
```

```output for=prototype-delegation
main: a rectangle
false
```

## Consequences

The main benefit is that the behaviour can change at run time. Design Patterns gives the same example: a window becomes circular when it replaces its rectangle with a circle, if both have the same type.[^2] The receiver also shows only the methods it chooses, not every method of the reused class.

The costs:

- **More code.** Each forwarded method is a method that someone must write and keep in step with the delegate, unless the language generates it.[^4]
- **Harder to follow.** Design Patterns warns that dynamic, highly parameterized software is harder to understand than static software, and that delegation has a cost at run time. It calls delegation a good design choice only when it simplifies more than it complicates.[^2]
- **The self problem.** Plain forwarding loses the sender. If the delegate calls one of its own methods, it calls its own version, not the override of the receiver. The Kotlin documentation says this of `by`: members that the class overrides do not get called from the members of the delegate.[^4]

Here the window overrides `name`, but the delegate calls its own `name`:

```python wrong id=lost-self
class Rectangle:
    def name(self):
        return "rectangle"

    def describe(self):
        return f"a {self.name()}"


class Window:
    def __init__(self, shape):
        self.shape = shape

    def name(self):
        return "window"

    def describe(self):
        return self.shape.describe()  # forwarding: self in the delegate is the shape


print(Window(Rectangle()).describe())
```

```output for=lost-self
a rectangle
```

The fix is to pass the receiver, so that the delegate calls back through it. This is the form that Design Patterns describes:[^2]

```python run id=pass-self
class Rectangle:
    def name(self):
        return "rectangle"

    def describe(self, owner):
        return f"a {owner.name()}"


class Window:
    def __init__(self, shape):
        self.shape = shape

    def name(self):
        return "window"

    def describe(self):
        return self.shape.describe(self)


print(Window(Rectangle()).describe())
```

```output for=pass-self
a window
```

## When not to use it

Do not delegate when the new class really is a kind of the old class and must be used where the old class is expected. Inheritance, or an interface that both classes implement, states that relation more directly.

Do not forward a large interface by hand when only one or two methods change. The forwarding code hides the methods that matter. Use a language feature such as Kotlin `by` or Go embedding, or reconsider the design.

Do not use `Deref` in Rust to imitate inheritance. The standard library advises to implement it only when the type transparently behaves like the target type, and not when the type has methods that are likely to collide with those of the target.[^8]

## Variants and related patterns

**Forwarding.** The receiver calls the delegate and does not pass itself. Most uses of the word "delegation" mean this form.[^1]

**Language-level delegation.** In prototype-based languages, delegation is the main way to reuse behaviour, as inheritance is in class-based languages.[^3] See [[prototype-based-programming]].

**Patterns built on delegation.** Design Patterns names State, Strategy and Visitor as patterns that depend on delegation.[^2] A [[strategy-pattern]] is a delegate that the client picks. A [[decorator-pattern]] and a [[proxy-pattern]] forward to an object with the same interface, and add or control behaviour.

**Composition over inheritance.** Delegation is the mechanism behind the principle of [[composition-over-inheritance]].

## Known uses

- Go: [bufio/bufio.go at go1.25.0](https://github.com/golang/go/blob/go1.25.0/src/bufio/bufio.go#L835-L845) defines `ReadWriter` as a struct that embeds a `*Reader` and a `*Writer`. The embedded fields promote their methods, so a `ReadWriter` implements `io.ReadWriter` with no forwarding methods.[^9][^5]
- Python: [Lib/tempfile.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/tempfile.py#L487-L527) wraps a file in `_TemporaryFileWrapper`. Its `__getattr__` delegates attribute lookups to the underlying file, and caches the methods it finds.[^10][^7]

## References
