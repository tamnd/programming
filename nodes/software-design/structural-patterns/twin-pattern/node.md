---
tag: T8KY09
kind: pattern
slug: twin-pattern
name: Twin
aliases: [twin pattern, twin object, twin class]
area: software-design/structural-patterns
level: research
summary: >-
  The Twin pattern models multiple inheritance in a language that has only single inheritance. Two linked objects each inherit from one parent, and together they act as one object.
family: structural-design
polarity: positive
intent: "Model multiple inheritance in a language without it: give each parent its own subclass, link the two subclass objects by fields, and let them act as one twin object."
also_known_as: []
history:
  people: [Hanspeter Mössenböck, Robert Griesemer]
  note: Mössenböck of the University of Linz described Twin as a design pattern in the form of the catalogue of Gamma, Helm, Johnson and Vlissides. Robert Griesemer discovered the technique in the implementation of a game program in Oberon.
see_also: [class-inheritance, composition-over-inheritance, delegation-pattern, mixin-pattern, adapter-pattern, template-method-pattern]
sources:
  - url: https://ssw.jku.at/Research/Papers/Moe99/Paper.pdf
    title: "Hanspeter Mössenböck, Twin: A Design Pattern for Modeling Multiple Inheritance, University of Linz"
    locator: sections 1 to 10 and Acknowledgements
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.1.4
    title: The Java Language Specification, Java SE 25 Edition, 8.1.4 Superclasses and Subclasses
    locator: "8.1.4, ClassExtends"
  - url: https://en.wikipedia.org/wiki/Twin_pattern
    title: Twin pattern, Wikipedia
    locator: Introduction; Definition; Collaborations
  - url: https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.desktop/share/classes/javax/swing/JComponent.java#L3736-L3745
    title: OpenJDK jdk-21-ga source, javax/swing/JComponent.java
    locator: JComponent at line 229, AccessibleJComponent at lines 3736 to 3745, getToolTipText at lines 4038 to 4040
---

## Intent

Twin is a structural pattern for languages with single inheritance, such as Java and Oberon-2. It models a class that inherits from two parents, and it avoids many of the problems of multiple inheritance.[^1]

## Problem

A class sometimes needs the data and code of two parent classes. Mössenböck's example is a ball game. All game items inherit from `GameItem`, which gives them methods to draw themselves and to check collisions. A ball is also an active object, so it must also inherit from `Thread`, which a scheduler runs.[^1]

In Java, the `extends` clause of a class names one class type, its direct superclass.[^2] A class `Ball` cannot extend both `GameItem` and `Thread`.

## Solution

Instead of one child class with two parents, make two child classes. Each child class inherits from one of the parents. The two child objects are linked by fields, so they can be seen as one twin object with two ends.[^3]

```text
   GameItem                    Thread
   (Parent1)                   (Parent2)
      ^                           ^
      |                           |
   BallItem  ---- twin ---->  BallThread
   (Child1)  <---- twin ----  (Child2)
   new fields and methods      overrides of Thread methods
```

The participants work as follows:[^1]

- Each child class is responsible for the protocol that it inherits from its parent. It handles the messages of this protocol and forwards other messages to its partner.
- A client refers to one twin directly, for example `ballItem`, and to the other through its twin field, for example `ballItem.twin`.
- A client that uses the protocol of one parent talks to the child of that parent. The game board talks to `BallItem`, and the scheduler talks to `BallThread`.
- New methods and fields are usually declared in one child only.

## Example

The example files show one program in each language. `Item` is a game item that prints its position with a template method `show`. `Ticker` counts ticks and calls a hook `on_tick` at each tick. A ball needs both parents, so `BallItem` extends `Item`, `BallTicker` extends `Ticker`, and each holds a reference to the other. Each tick moves the ball by 2. The program prints:

```text
ball at 0
ball at 2
ball at 4
ticks: 2
```

C++ has multiple inheritance, Go has struct embedding, and Rust has traits with default methods, so their files show the direct form instead of a twin.

## Consequences

The benefits:[^1]

- A single-inheritance language gets most of the benefits of multiple inheritance.
- Each child keeps the names of its own parent, so the name clashes of multiple inheritance do not occur.

The costs:

- **Forwarding.** Twin replaces inheritance relations with composition. Messages must be forwarded, and forwarding is less efficient than inheritance.[^1]
- **Close coupling.** The two children often need each other's private fields and methods. In Java, the children can share a package and use package visibility. In Modula-3 and Oberon, they can share a module.[^1]
- **Subclassing.** It is often sufficient to subclass one twin. The subclass is then compatible with that twin only, not with its partner.[^1]

Here the twin is subclassed on the item side only. `FastBall` overrides `on_tick` to move by 5, but the ticker calls its own `on_tick`, so the override has no effect:

```python wrong id=one-sided-subclass
class Item:
    def __init__(self, name):
        self.name = name

    def show(self):
        print(f"{self.name} at {self.position()}")


class Ticker:
    def tick(self):
        self.on_tick()


class BallItem(Item):
    def __init__(self, name):
        super().__init__(name)
        self.x = 0
        self.twin = BallTicker(self)

    def position(self):
        return self.x

    def on_tick(self):  # a copy of the Ticker protocol, for subclasses
        self.twin.on_tick()


class BallTicker(Ticker):
    def __init__(self, twin):
        self.twin = twin

    def on_tick(self):
        self.twin.x += 2


class FastBall(BallItem):
    def on_tick(self):  # meant to move by 5
        self.x += 5


ball = FastBall("fast ball")
ball.twin.tick()  # the scheduler holds the ticker end of the twin
ball.show()
```

```output for=one-sided-subclass
fast ball at 2
```

One fix is to keep the behaviour in the child that has the new fields and to let the other child forward to it. A subclass of that child then changes the behaviour for both ends:

```python run id=forward-to-item
class Item:
    def __init__(self, name):
        self.name = name

    def show(self):
        print(f"{self.name} at {self.position()}")


class Ticker:
    def tick(self):
        self.on_tick()


class BallItem(Item):
    def __init__(self, name):
        super().__init__(name)
        self.x = 0
        self.twin = BallTicker(self)

    def position(self):
        return self.x

    def move(self):
        self.x += 2


class BallTicker(Ticker):
    def __init__(self, twin):
        self.twin = twin

    def on_tick(self):
        self.twin.move()  # forward to the partner


class FastBall(BallItem):
    def move(self):
        self.x += 5


ball = FastBall("fast ball")
ball.twin.tick()
ball.show()
```

```output for=forward-to-item
fast ball at 5
```

The other fix is to make the subclass a twin again, with one grandchild for each child.[^1]

## When not to use it

- The language has multiple inheritance, traits or mixins that solve the problem directly.
- The class needs only the type of the second parent, not its code. In Java, the class can then extend one class and implement an interface, as `MyApplet extends Applet implements MouseListener` does in Mössenböck's paper.[^1]
- More than two parents. Twin extends to three or more parents, with one child for each parent and links between all children. This is much more complex than multiple inheritance.[^1]

## Variants and related patterns

**Inner class.** In Java, one twin can be an inner class of the other. The inner object can then use all private fields of the outer object, and no explicit link is necessary. The solution is not symmetric: the outer class cannot use the private fields of the inner class.[^1]

**Adapter.** Twin is related to the two-way [[adapter-pattern]] of Gamma and his coauthors, which lets two clients view one object differently. The two-way adapter uses multiple inheritance, and Twin avoids it.[^1]

**Delegation.** Each twin forwards the messages that it does not handle to its partner, as in the [[delegation-pattern]].[^1]

**Mixin.** In a language with mixins, a [[mixin-pattern]] gives a class the code of a second class without a twin.

## Known uses

- Oberon-2: the ball game of the motivating example was a teaching exercise. In the Oberon system, a ball was a process and also a game object.[^1]
- Java applets: an applet extends `Applet`. A partner `MyAppletListener` extends a standard listener class `StdMouseListener`, and each holds a link to the other.[^1]
- Java Swing: [JComponent.java at jdk-21-ga](https://github.com/openjdk/jdk/blob/jdk-21-ga/src/java.desktop/share/classes/javax/swing/JComponent.java#L3736-L3745) has the inner class form. `JComponent` extends `Container`, and its inner class `AccessibleJComponent` extends `AccessibleAWTContainer`. Its `getToolTipText` method forwards to `JComponent.this.getToolTipText()`.[^4]

## References
