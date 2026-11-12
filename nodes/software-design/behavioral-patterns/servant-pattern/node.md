---
tag: T6ZF2X
kind: pattern
slug: servant-pattern
name: Servant
aliases: [servant pattern, servant design pattern]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  A servant is a class that gives one behaviour to a group of classes. The served objects implement a small interface, and the servant takes them as parameters.
family: behavioral-design
polarity: positive
intent: Define a behaviour once in a servant class, and offer it to every class that implements the interface the servant requires.
also_known_as: []
history:
  year: 2006
  people: [Rudolf Pecinovský, Jarmila Pavlíčková, Luboš Pavlíček]
  note: Pecinovský, Pavlíčková and Pavlíček used the name Servant for this pattern in a 2006 ITiCSE paper on teaching programming with design patterns first. They say it is in reality another type of the Command pattern.
see_also: [command-pattern, strategy-pattern, visitor-pattern, interfaces]
sources:
  - url: https://en.wikipedia.org/wiki/Servant_(design_pattern)
    title: Servant (design pattern), Wikipedia
    locator: lead; Description and simple example; Two ways of implementation; Similar design pattern, Command
  - url: http://edu.pecinovsky.cz/papers/2006_ITiCSE_Design_Patterns_First.pdf
    title: Rudolf Pecinovský, Jarmila Pavlíčková and Luboš Pavlíček, Let's Modify the Objects-First Approach into Design-Patterns-First, ITiCSE 2006
    locator: section 3, Second lesson; section 3.3, Introducing interface; section 4.1
  - url: https://github.com/golang/go/blob/go1.25.0/src/sort/sort.go#L15-L50
    title: Go go1.25.0 source, src/sort/sort.go
    locator: type Interface and func Sort, lines 15 to 50
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Collections.html
    title: Java SE 25 API, java.util.Collections
    locator: class description; sort(List)
---

## Intent

The servant pattern defines an object used to offer some functionality to a group of classes without defining that functionality in each of them.[^1]

## Problem

Some objects must all do a common action. In the Wikipedia example, there are classes for geometric objects, such as a rectangle, an ellipse and a triangle, and each of them must move on a canvas.[^1]

You can write a move method in each class. Then the same code is in many places. You can also put the method in a common parent class, but this is not always possible.[^1] The paper says: "When we want some objects to perform a common action, it is often not the best solution to define this action as a method in every class."[^2]

## Solution

Define a special class, the servant, that serves all these objects and provides the common functionality.[^2] The servant needs some abilities from the served objects. An interface states these requirements, and each served class implements it.[^2]

```text
   User ------------> MoveServant
                        move_to(movable, where)
                        move_by(movable, dx, dy)
                             |
                             | uses only the interface
                             v
                      <<interface>> Movable
                        get_position()
                        set_position(p)
                        ^      ^       ^
               Rectangle   Ellipse   Triangle
```

In the example, the interface `Movable` has the methods `getPosition` and `setPosition`. The servant `MoveServant` has the methods `moveTo` and `moveBy`. It can move every object that implements `Movable`, so the moving code appears in only one class.[^1]

## Example

The example files show one program in each language. Three shapes implement `Movable`. One `MoveServant` moves all of them, and no shape has its own move code. The program prints:

```text
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

## Consequences

The benefits:

- The common behaviour is in one class.[^1] A change to the behaviour is made in one place.
- The served classes need not have a common parent class. They need only implement the interface.[^1]
- The served classes do not have to know about the servant, in the first form of the pattern.[^1]

The costs:

- The servant can use only what the interface gives. The served classes must make that data public through the interface, for example the position of a shape.[^1]
- There is one more class and one more interface to understand.

Without a servant, each class has its own copy of the code, and the copies can become different. Here the ellipse copy has an error, and only the ellipse moves the wrong way:

```python wrong id=copies
class Rectangle:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def move_by(self, dx, dy):
        self.x, self.y = self.x + dx, self.y + dy


class Ellipse:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def move_by(self, dx, dy):
        self.x, self.y = self.x + dx, self.y - dy  # a copy with an error


for shape in [Rectangle(0, 0), Ellipse(10, 0)]:
    shape.move_by(3, 4)
    print(type(shape).__name__.lower(), f"at ({shape.x}, {shape.y})")
```

```output for=copies
rectangle at (3, 4)
ellipse at (13, -4)
```

## When not to use it

- Only one class needs the behaviour. Then a method in that class is simpler.
- The behaviour needs private data of each class that the class must not make public.
- A common parent class can hold the behaviour, and the classes already share that parent.[^1]

## Variants and related patterns

There are two ways to implement the pattern:[^1]

1. The user knows the servant. It calls the servant and gives it the served objects as parameters. The served classes do not know about the servant.
2. The served objects know the servant. The user calls the served objects, and they ask the servant to do the work.

The second form looks like this. The user calls `move_by` on the shape, and the shape sends the request to the servant:

```python run id=served-asks
class MoveServant:
    def move_by(self, movable, dx, dy):
        x, y = movable.get_position()
        movable.set_position((x + dx, y + dy))


SERVANT = MoveServant()


class Triangle:
    def __init__(self, position):
        self._position = position

    def get_position(self):
        return self._position

    def set_position(self, position):
        self._position = position

    def move_by(self, dx, dy):
        SERVANT.move_by(self, dx, dy)  # the shape asks the servant


t = Triangle((0, 10))
t.move_by(3, 4)
print("triangle at", t.get_position())
```

```output for=served-asks
triangle at (3, 14)
```

- **Command.** The two patterns are very similar, and implementations of them are often virtually the same. The difference is the approach to the problem. With Servant, a class offers functionality, and the served objects are passed to it. With Command, command objects are passed to the original objects as parameters of their methods.[^1] The 2006 paper calls Servant in reality another type of Command.[^2] Its teaching example is a servant `Repeater` with a method `repeat(int, Command)` that calls `command()` of its second parameter the given number of times.[^2]

## Known uses

- Go's package `sort` has the interface `sort.Interface` with the methods `Len`, `Less` and `Swap`. The comment says that an implementation of `Interface` can be sorted by the routines in the package. The function `Sort(data Interface)` sorts any such value, so the sorted types do not contain sort code.[^3]
- Java's class `java.util.Collections` consists exclusively of static methods that operate on or return collections. Its method `sort(List)` requires that all elements in the list implement the `Comparable` interface.[^4]

## References
