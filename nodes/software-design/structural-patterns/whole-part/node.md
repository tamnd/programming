---
tag: TFBD5H
kind: pattern
slug: whole-part
name: Whole-Part
aliases: [whole-part pattern, aggregate component, whole and parts]
area: software-design/structural-patterns
level: advanced
summary: >-
  The Whole-Part pattern aggregates components that form a semantic unit. The whole encapsulates its parts, organizes their collaboration, and gives clients one interface to them.
family: structural-design
polarity: positive
intent: "Aggregate components that together form a semantic unit into a whole that encapsulates the parts, organizes their collaboration and gives clients one interface."
also_known_as: []
history:
  year: 1996
  people: [Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad, Michael Stal]
  note: Whole-Part is one of the patterns of Pattern-Oriented Software Architecture, which John Wiley and Sons published in 1996. An OOPSLA 98 workshop on non-software examples of these patterns gave a car as the example of Whole-Part.
see_also: [composite-pattern, facade-pattern, encapsulation-and-visibility, composition-over-inheritance, delegation-pattern]
sources:
  - book: isbn:9780471958697
    title: "Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad and Michael Stal, Pattern-Oriented Software Architecture, Volume 1: A System of Patterns, John Wiley and Sons"
    locator: Whole-Part
  - url: https://www.cs.uni.edu/~wallingf/teaching/062/sessions/support/pattern-examples.pdf
    title: "Examples to Accompany: Design Patterns, Elements of Reusable Object-Oriented Software, AG Communication Systems, 1999"
    locator: "introduction to the PoSA examples, page A-36; Whole-Part, page A-44"
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Composite and Facade
  - url: https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.base/share/classes/java/util/concurrent/ThreadPoolExecutor.java#L454-L475
    title: OpenJDK jdk-21-ga source, java/util/concurrent/ThreadPoolExecutor.java
    locator: class at line 329, workQueue at line 454, workers at line 475, Worker at line 608, execute at line 1339
---

## Intent

Whole-Part is a structural pattern from Pattern-Oriented Software Architecture.[^1] It helps aggregate components that form a semantic unit.[^2] The aggregate is the whole, and the components are its parts. The whole encapsulates the parts, organizes how they work together, and gives clients one interface.

## Problem

A car is made of many parts: a fuel injector, a distributor, spark plugs and pistons. When the car is driven, these parts work together in a fixed order. The driver has no direct access to any of them, and uses all of them through the controls of the car.[^2]

Software has the same shape. Some objects are only useful together, and the rules of their collaboration belong to the group, not to each client. If each client reaches into the parts, each client must know the rules, and a change to one part breaks the clients.

## Solution

Make the whole an object of its own. It creates or receives the parts, keeps them private, and gives an interface in terms of the unit. Each operation of the whole calls the parts in the right order. The participants of the car example are these:[^2]

- The client is the driver, who uses all of the components through the interface of the whole.
- The whole is the car.
- The parts are the smaller systems: the fuel system, the ignition system and the cooling system.

```text
   Client
     |  drive()
     v
   +------------------- Whole (Car) --------------------+
   |   drive(): starter.crank(), engine.fire(),          |
   |            wheels.turn()                            |
   |                                                     |
   |   Starter        Engine          Wheels             |
   |   (part)         (part)          (part)             |
   +-----------------------------------------------------+
       the client has no reference to a part
```

## Example

The example files show one program in each language. A `Car` creates a starter, an engine and wheels, and keeps them private. Its `drive` operation calls the three parts in order. The car then gets a new starter. The client code does not change, and the output shows the new part:

```text
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

## Consequences

The benefits:[^2]

- **Changeable parts.** A part can change without an effect on the client. If a starter fails, it is replaced, and the interface of the car stays the same for the driver.
- **Separate concerns.** Each concern is in a separate part, which makes complex strategies easier to implement. Many parts of a car are simple, but the whole is complex.
- **Reuse.** The parts of one whole can be used in other aggregates. The auto salvage industry takes a part from one car for use in another.

The costs:

- **A larger interface on the whole.** Each service that a client needs from a part must be an operation of the whole.
- **Indirection.** Each call goes through the whole, which forwards it to the parts.

The encapsulation is what keeps the whole correct. Here an order is a whole with lines as parts, and it keeps a running total. The order lets clients reach its list of lines, so a client adds a line directly, and the total is wrong:

```python wrong id=exposed-parts
class Order:
    def __init__(self):
        self.lines = []  # the parts, open to every client
        self.total = 0

    def add_line(self, name, price):
        self.lines.append((name, price))
        self.total += price


order = Order()
order.add_line("pen", 4)
order.add_line("ink", 6)
order.lines.append(("paper", 5))  # bypasses the whole
print(f"lines: {len(order.lines)}, total: {order.total}")
```

```output for=exposed-parts
lines: 3, total: 10
```

When the whole keeps the parts private and gives a read-only copy, every change goes through the whole:

```python run id=hidden-parts
class Order:
    def __init__(self):
        self._lines = []
        self._total = 0

    def add_line(self, name, price):
        self._lines.append((name, price))
        self._total += price

    @property
    def lines(self):
        return tuple(self._lines)  # a copy: no client can change the parts

    @property
    def total(self):
        return self._total


order = Order()
order.add_line("pen", 4)
order.add_line("ink", 6)
order.add_line("paper", 5)
print(f"lines: {len(order.lines)}, total: {order.total}")
```

```output for=hidden-parts
lines: 3, total: 15
```

## When not to use it

- Clients need direct access to the parts, for example to use them in their own order. A [[facade-pattern]] gives a simple interface and still lets these clients use the parts.
- The parts do not form a unit, and there are no rules for their collaboration. A whole then only forwards calls.
- There is only one part. A wrapper around one object is a [[delegation-pattern]] or an adapter, not an aggregate.

## Variants and related patterns

**Composite.** The [[composite-pattern]] composes objects into tree structures that represent part-whole hierarchies, and clients treat single objects and compositions in the same way.[^3] A composite shows its children to clients. A whole hides its parts.

**Facade.** A [[facade-pattern]] gives one interface to a set of interfaces in a subsystem.[^3] The subsystem classes stay available to the clients that need them, so a facade does not give the encapsulation of a whole.

**Composition over inheritance.** A whole gets its behaviour from parts that it holds, not from parent classes, as in [[composition-over-inheritance]].

## Known uses

- Java: [ThreadPoolExecutor.java at jdk-21-ga](https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.base/share/classes/java/util/concurrent/ThreadPoolExecutor.java#L454-L475) keeps its parts in private fields: a `BlockingQueue<Runnable> workQueue` and a `HashSet<Worker> workers`. `Worker` is a `private final class`. A client gives a task to the pool through `execute(Runnable command)`, and the task runs "in a new thread or in an existing pooled thread".[^4]
- A car: the OOPSLA 98 workshop example. The driver uses the fuel, ignition and cooling systems only through the car.[^2]

## References
