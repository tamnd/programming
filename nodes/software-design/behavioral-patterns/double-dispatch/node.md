---
tag: VEMJGW
kind: pattern
slug: double-dispatch
name: Double dispatch
aliases: [double dispatching, multiple polymorphism]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  Double dispatch selects a method by the run-time types of two objects. In a single-dispatch language, the first call dispatches on the receiver, and it calls back the argument, which dispatches again.
family: behavioral-design
polarity: positive
intent: "Dispatch a call to different concrete functions depending on the run-time types of two objects, in a language whose calls dispatch on the receiver only."
also_known_as: [multiple polymorphism]
history:
  year: 1986
  people: [Dan Ingalls]
  note: Ingalls first described how to use double dispatching in Smalltalk, and called it multiple polymorphism, in A Simple Technique for Handling Multiple Polymorphism at OOPSLA '86.
see_also: [dynamic-dispatch, visitor-pattern, specification, strategy-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Double_dispatch
    title: Double dispatch, Wikipedia
    locator: Introduction; Overview; Use cases; A common idiom; Examples, C++ and C#
  - url: https://docs.python.org/3/library/functools.html#functools.singledispatch
    title: The Python Standard Library, functools, functools.singledispatch
    locator: functools.singledispatch
  - url: https://docs.python.org/3/reference/datamodel.html#object.__radd__
    title: The Python Language Reference, 3.3.8 Emulating numeric types
    locator: object.__radd__ and the other reflected methods
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/fractions.py#L616-L724
    title: CPython v3.14.0 source, Lib/fractions.py
    locator: Fraction._operator_fallbacks, forward and reverse, lines 616 to 724
---

## Intent

Double dispatch is a special form of multiple dispatch. It dispatches a function call to different concrete functions, depending on the run-time types of two objects that are involved in the call.[^1]

## Problem

In most object-oriented systems, the function that a call runs depends on the dynamic type of a single object, the receiver. These calls are single dispatch calls, or virtual function calls.[^1] Sometimes the correct computation depends on the types of two objects. In a game, the collision between a spaceship and an asteroid is computed differently from the collision between a spaceship and a space station.[^1] Other use cases are the sorting of a mixed set of objects, event handling that depends on the event type and the receiver type, and lock and key systems.[^1]

Function overloading looks like a solution, but in C++ overloading is resolved at compile time. A call through a reference to `Spaceship` selects the overload for `Spaceship`, even when the object is an `ApolloSpacecraft`.[^1] Python's `functools.singledispatch` has the same limit at run time. The dispatch happens on the type of the first argument only:[^2]

```python wrong id=first-argument-only
from functools import singledispatch


class Ship: pass
class Asteroid: pass


@singledispatch
def collide(a, b):
    return "unknown"


@collide.register
def _(a: Ship, b):
    return "ship takes damage"  # the type of b is not known here


@collide.register
def _(a: Asteroid, b):
    return "asteroid hits something"  # the type of b is not known here


print(collide(Asteroid(), Ship()))
print(collide(Asteroid(), Asteroid()))  # meant: asteroids break apart
```

```output for=first-argument-only
asteroid hits something
asteroid hits something
```

## Solution

Systems such as CLOS have multiple dispatch. In other systems, double dispatch is a way to get the same result with two single dispatches.[^1] The first call dispatches on the receiver. The receiver then calls a method of the argument and passes itself. The name of that second method tells the type of the first object, and the second call dispatches on the type of the argument.

```text
  a.collide(b)                   first dispatch: on the type of a
     |
     +--> b.collide_with_asteroid(a)   if a is an Asteroid
     +--> b.collide_with_ship(a)       if a is a Ship
                 |
                 +--> second dispatch: on the type of b
```

In the C++ example of Wikipedia, each spaceship class gets a virtual function `collideWith(Asteroid& asteroid)` that calls `asteroid.collideWith(*this)`. The type of `*this` is static in each class, so overloading now selects the correct function.[^1]

## Example

The example files show one program in each language. `Ship` and `Asteroid` each have `collide`, which calls back the other object with a method named for its own type. The program collides four pairs and prints:

```text
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

## Consequences

The benefits:

- The choice of computation depends on the run-time types of both arguments, with no type tests in the client.[^1]

The costs:

- **Run-time cost.** The call has the usual costs of dynamic resolution, usually more than in a language with single dispatch only.[^1]
- **Many methods.** In the example, each class has one callback method for each class that it can meet. A third class, such as a space station, adds a callback method to each existing class.
- **Order.** The two objects of a call have different roles. In the example, `ship + asteroid` and `asteroid + ship` must give the same result, so two methods must agree.

## When not to use it

- The language has multiple dispatch, for example CLOS.[^1] Then one method can be declared for each pair of types.
- In C#, a call can do multiple dispatch without the Visitor pattern: the method uses traditional polymorphism and casts the argument to `dynamic`.[^1]
- Only one of the two types varies. Then single dispatch on that object is sufficient.

## Variants and related patterns

- **Visitor.** The problem of the C++ example can be resolved by simulating double dispatch, for example with the Visitor pattern.[^1]
- **Reflected operators in Python.** To evaluate `x - y`, Python calls `type(y).__rsub__(y, x)` if `type(x).__sub__(x, y)` returns `NotImplemented`, or if `type(y)` is a subclass of `type(x)`.[^3] So a binary operator can dispatch first on the left operand and then on the right operand:

```python run id=reflected
class Meters:
    def __init__(self, n):
        self.n = n

    def __add__(self, other):
        if isinstance(other, Meters):
            return Meters(self.n + other.n)
        if isinstance(other, int):
            return Meters(self.n + other)
        return NotImplemented

    __radd__ = __add__

    def __repr__(self):
        return f"Meters({self.n})"


print(Meters(2) + 3)  # Meters.__add__
print(3 + Meters(2))  # int.__add__ returns NotImplemented, then Meters.__radd__
print(sum([Meters(1), Meters(2)]))  # sum starts with 0 + Meters(1)
```

```output for=reflected
Meters(5)
Meters(5)
Meters(3)
```

## Known uses

- Dan Ingalls described double dispatching in Smalltalk in 1986.[^1]
- CPython's `fractions.Fraction` makes its arithmetic operators with `_operator_fallbacks`, which returns a forward and a reverse function. The comment says that mixed-mode operations should "call an implementation whose author knew about the types of both arguments", or convert both to the nearest built in type. The functions return `NotImplemented` for types that they do not know.[^4]

## References
