---
tag: NEW-abstract-factory-pattern
kind: pattern
slug: abstract-factory-pattern
name: Abstract Factory pattern
aliases: [Abstract Factory, Kit, factory object]
area: software-design/creational-patterns
level: core
summary: >-
  One object has a factory method for each kind of product in a family. Client code gets all of its products from that object, so it never names a concrete class and never mixes two families.
family: creational-design
polarity: positive
intent: "Give an interface for creating families of related objects, so that client code creates them without naming their concrete classes, and so that one choice selects the whole family."
also_known_as: [Kit]
refactorings: []
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Kit as a second name and a user interface toolkit with several look-and-feel standards as the motivating example.
see_also: [factory-method-pattern, builder-pattern, singleton-pattern, strategy-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Abstract Factory (Intent, Also Known As, Motivation, Participants, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (first-class types make Abstract-Factory simpler) and slides 12 and 13 (Abstract Factory in a dynamic language)
  - url: https://peps.python.org/pep-0249/#module-interface
    title: PEP 249, Python Database API Specification v2.0
    locator: Module Interface, Constructors (connect returns a Connection object)
  - url: https://peps.python.org/pep-0249/#connection-objects
    title: PEP 249, Python Database API Specification v2.0
    locator: Connection Objects, Connection methods (.cursor)
  - url: https://pkg.go.dev/database/sql/driver#Driver
    title: Go standard library, database/sql/driver
    locator: type Driver; type Conn
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.data.common.dbproviderfactory
    title: .NET API reference, System.Data.Common.DbProviderFactory
    locator: class description and methods
  - url: https://github.com/dotnet/runtime/blob/v10.0.0/src/libraries/System.Data.Common/src/System/Data/Common/DbProviderFactory.cs#L9-L72
    title: dotnet/runtime v10.0.0 source, System.Data.Common/DbProviderFactory.cs
    locator: class DbProviderFactory, lines 9 to 72
  - url: https://docs.python.org/3.14/library/abc.html#abc.abstractmethod
    title: The Python Standard Library, abc, Python 3.14
    locator: abc.abstractmethod
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Builder (Related Patterns)
  - url: https://www.informit.com/articles/article.aspx?p=1404056
    title: Larry O'Brien, Design Patterns 15 Years Later, An Interview with Erich Gamma, Richard Helm, and Ralph Johnson, InformIT, October 22, 2009
    locator: Gamma's draft of a revised catalog (generalize Factory Method to Factory; add Dependency Injection)
---

## Intent

Give an interface for creating families of related or dependent objects without naming their concrete classes.[^1] The client asks one object for each product it needs. The program chooses that object once, and the choice fixes the family of every product the client gets.

## Problem

A program draws a form with a button and a checkbox. It supports two visual styles. In the plain style, a button is `[ Save ]` and a checked box is `[x]`. In the round style, they are `( Save )` and `(*)`. A form must not mix the styles.

The first version tests the style where it creates each widget. That test is repeated at each place that creates a widget. When the program gains the round style, one place is missed, and the form mixes the families:

```python wrong id=mixed-family
def plain_button(label):
    return f"[ {label} ]"


def plain_checkbox(label, checked):
    return f"[{'x' if checked else ' '}] {label}"


def round_button(label):
    return f"( {label} )"


def round_checkbox(label, checked):
    return f"({'*' if checked else ' '}) {label}"


def login_form(style):
    button = round_button("Save") if style == "round" else plain_button("Save")
    checkbox = plain_checkbox("Remember me", True)  # this line was not updated for "round"
    return f"{button} {checkbox}"


print(login_form("round"))
```

```output for=mixed-family
( Save ) [x] Remember me
```

Design Patterns uses the same problem: a user interface toolkit that supports several look-and-feel standards, such as Motif and Presentation Manager.[^1] The forces are these. The client must not depend on concrete product classes. The products of one family must be used together. And the program should change families in one place.

## Solution

Design Patterns names five participants.[^1]

- **AbstractFactory** declares one creation operation for each kind of product.
- **ConcreteFactory** implements the operations for one family.
- **AbstractProduct** declares the interface of one kind of product.
- **ConcreteProduct** is a product of one family, made by the matching concrete factory.
- **Client** uses only the abstract factory and the abstract products.

The program makes one concrete factory, often at startup, and passes it to the client. The client calls `button` and `checkbox` on it and never names `PlainButton` or `RoundCheckbox`. All products come from one object, so they come from one family. Each creation operation is usually a factory method, so an abstract factory is a set of factory methods on one object.[^1] The [[factory-method-pattern]] page covers that part.

The choice of the family moves to one line, the line that makes the concrete factory. The rest of the program depends on interfaces only.

## Example

The example files draw the same login form with both families. The output is `plain: [ Save ] [x] Remember me`, then `round: ( Save ) (*) Remember me`. The function `login_form` is the client and is the same for both families:

| Language | Form |
|---|---|
| Python | abstract base classes with `abc.abstractmethod`[^8] |
| JavaScript | a frozen object literal of factory functions for each family |
| TypeScript | an interface that each factory class implements |
| Java | interfaces for the factory and the products, and records for the products |
| C# | interfaces and records |
| C++ | abstract classes, with `std::unique_ptr` for the products |
| Go | an interface, with unexported product types |
| Rust | a trait with associated types for the products, and a generic client |

In the Rust file, the trait names the product types of each family. A generic client `login_form<F: WidgetFactory>` gets `F::Button` and `F::Checkbox`, so the compiler knows that both come from the same family.

## Consequences

Design Patterns lists four consequences.[^1] The pattern isolates concrete classes, because the client handles products only through their interfaces. It makes a change of product family a small change, because the concrete factory appears once in the program. It promotes consistency among products, because a client that uses one factory gets one family. These are the benefits.

The fourth consequence is the liability. Support for a new kind of product is hard to add. The abstract factory fixes the set of products it can create. A new kind, such as a slider, changes the abstract factory and every concrete factory.[^1] In Python, an abstract base class makes this visible. A concrete factory that lacks the new method cannot be created:[^8]

```python fails expect=exception:TypeError id=new-product
from abc import ABC, abstractmethod


class WidgetFactory(ABC):
    @abstractmethod
    def button(self, label): ...

    @abstractmethod
    def checkbox(self, label, checked): ...

    @abstractmethod
    def slider(self, label, value): ...  # the new kind of product


class PlainFactory(WidgetFactory):
    def button(self, label):
        return f"[ {label} ]"

    def checkbox(self, label, checked):
        return f"[{'x' if checked else ' '}] {label}"


PlainFactory()
```

In Java, C#, TypeScript, Go and Rust, the same change is a compile error for each concrete factory. The error is the useful part: it lists each family that needs the new product. The cost grows with the number of families times the number of products.

A library with factories that it does not own cannot make that change. The .NET class `DbProviderFactory` shows one way out. It is an abstract class, and its creation methods are virtual with a default. `CreateConnection`, `CreateCommand` and `CreateParameter` return `null` when a provider does not override them. `CreateBatch` throws `NotSupportedException`, and the property `CanCreateBatch` is `false` by default.[^7] A provider written before batches existed still compiles, and a client asks `CanCreateBatch` before it calls `CreateBatch`. The price is a check at run time in place of a compile error. A Java interface can do the same with `default` methods.

The pattern also adds types. Two families of two products need an abstract factory, two concrete factories, two product interfaces and four product classes. The pattern pays off when the client is large and the families are real. It does not pay off for one family with a second one that may come later.

**One factory for the whole program.** A concrete factory usually has no state, and a program usually needs one for each family. Design Patterns notes that a concrete factory is often a singleton.[^1] Make one instance at startup and pass it in. The [[singleton-pattern]] page explains why a global instance is often worse than a parameter.

## When not to use it

Do not use the pattern for one family. A direct constructor call is clearer, and adding the factory later is a local change if the client code takes its products from few places.

Do not use it when the products of different families can be mixed safely. The pattern exists to keep a family together. If any button can go with any checkbox, pass each factory function or class on its own.

Do not use it when the set of products changes often and the set of families is stable. Each new product changes every factory. When the products vary more than the families, group the code by product. The trade-off is the expression problem, which the [[composite-pattern]] page describes.

Do not write a class hierarchy of factories in a language where a class or a module is a value. A record of classes or a module does the same work.

## Variants and related patterns

**Classes or modules as factories.** Norvig points out that in a language with first-class types, a class is already a factory. The program can store classes in variables and needs no parallel hierarchy of factory classes. He also notes that a programmer may still want a factory-like object that bundles the classes of one family.[^2] In Python, that object can be a namespace of classes:

```python run id=namespace-kit
from types import SimpleNamespace


class PlainButton:
    def __init__(self, label):
        self.label = label

    def render(self):
        return f"[ {self.label} ]"


class RoundButton(PlainButton):
    def render(self):
        return f"( {self.label} )"


kits = {
    "plain": SimpleNamespace(button=PlainButton),
    "round": SimpleNamespace(button=RoundButton),
}
widgets = kits["round"]  # the one place that chooses the family
print(widgets.button("Save").render())
```

```output for=namespace-kit
( Save )
```

A Python module can be the kit too. PEP 249 has this shape. Each database module has a `connect` constructor that returns a connection, and the connection's `cursor` method returns a cursor of the same driver.[^3][^4] The program imports one module, and every object it gets from the module belongs to that driver. Go's `database/sql/driver` package is the same idea with interfaces. A `Driver` opens a `Conn`, and the connection prepares statements of its own driver.[^5]

**Dependency injection.** A dependency injection container is a general factory. The program registers the concrete classes in one place, and the container passes them to the classes that need them. In his 2009 draft of a revised catalog, Gamma proposed to add Dependency Injection as a pattern.[^10] With a container, the abstract factory often becomes one registration of the family's classes.

**Factory Method.** An abstract factory is often a set of factory methods. The difference is the place of the choice. In Factory Method, a subclass of the client chooses the product. In Abstract Factory, the client receives an object that chooses.[^1] See [[factory-method-pattern]].

**Builder.** Design Patterns contrasts the two. Abstract Factory makes a family of products, and each product returns at once. Builder makes one complex product step by step and returns it as a final step.[^9] See [[builder-pattern]].

**Prototype.** A concrete factory can hold one prototype instance of each product and copy it, in place of a subclass for each family. Design Patterns describes this in its Implementation section.[^1]

**Strategy.** An abstract factory is a strategy for creation. A client that receives a factory has one of its decisions made by another object, as a client of a strategy does. The [[strategy-pattern]] page covers the general case.

## Known uses

- .NET: [DbProviderFactory at v10.0.0](https://github.com/dotnet/runtime/blob/v10.0.0/src/libraries/System.Data.Common/src/System/Data/Common/DbProviderFactory.cs#L9-L72) is an abstract class with creation methods for the connection, command, parameter, command builder, data adapter and batch classes of one data provider. Each provider derives a concrete factory. The methods are virtual with a default, which keeps old providers working when a product is added.[^6][^7]
- Python: the DB-API of PEP 249. Each driver module is a kit whose `connect` returns a connection, and whose connection returns cursors of the same driver.[^3][^4]
- Go: the [`database/sql/driver`](https://pkg.go.dev/database/sql/driver#Driver) interfaces. A driver opens connections, and a connection prepares statements and begins transactions of the same driver.[^5]

## References
