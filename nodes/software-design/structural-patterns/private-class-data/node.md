---
tag: MG3C36
kind: pattern
slug: private-class-data
name: Private class data
aliases: [private class data pattern, data object, private data class]
area: software-design/structural-patterns
level: advanced
summary: >-
  Private class data moves the fields of a class into a separate data object that only lets them be read. Even the methods of the class cannot then change a field that was meant to be set once.
family: structural-design
polarity: positive
intent: "Control write access to the attributes of a class by moving them into a separate data object, which the class creates in its constructor and which exposes the attributes only for reading."
also_known_as: [Data object]
see_also: [immutability, encapsulation-and-visibility, constructors, builder-pattern]
sources:
  - url: https://sourcemaking.com/design_patterns/private_class_data
    title: Private Class Data Design Pattern, SourceMaking
    locator: Intent; Problem; Discussion; Structure; Check list
  - url: https://docs.python.org/3.14/library/dataclasses.html
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: frozen; FrozenInstanceError; Frozen instances
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.10.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.10.3 Record Members (component fields)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/init
    title: The init keyword, C# language reference
    locator: introduction
  - url: https://doc.rust-lang.org/book/ch07-03-paths-for-referring-to-an-item-in-the-module-tree.html
    title: The Rust Programming Language, Paths for Referring to an Item in the Module Tree
    locator: Making Structs and Enums Public
  - url: https://github.com/golang/go/blob/go1.25.0/src/os/types.go#L14-L20
    title: Go go1.25.0 source, src/os/types.go
    locator: type File, lines 14 to 20
  - url: https://github.com/golang/go/blob/go1.25.0/src/os/file_unix.go#L55-L66
    title: Go go1.25.0 source, src/os/file_unix.go
    locator: type file, lines 55 to 66
---

## Intent

Control write access to the attributes of a class, and separate the data from the methods that use it.[^1] The class moves its fields into a separate **data object**. The class creates the data object in its constructor, and the data object lets the fields be read but not written.

## Problem

An `Order` has an id and a customer, which must not change after the order is created, and a list of items, which changes. All three are fields of the class, so every method of the class can write all three. A method that merges two orders copies the items, and, by mistake, also the id:

```python wrong id=merge-changes-id
class Order:
    def __init__(self, order_id, customer):
        self.order_id = order_id
        self.customer = customer
        self.items = []

    def merge(self, other):
        self.items += other.items
        self.order_id = other.order_id  # bug: the id must not change


a = Order(42, "ann")
b = Order(43, "bob")
b.items.append("pen")
a.merge(b)
print("order", a.order_id, "for", a.customer)
```

```output for=merge-changes-id
order 43 for ann
```

SourceMaking gives the general case: a class may expose its attributes to manipulation when manipulation is no longer desirable, for example after construction.[^1] Some fields are set once but cannot be declared final, because the language has no such modifier, or because the value is computed in the constructor.

The forces are these:

- Some fields must not change after construction, and some must.
- The language cannot mark a field as final, or its final applies only to the reference and not to the object.
- The protection must also hold against the methods of the class itself.

## Solution

Create a data class and move into it every attribute that needs protection. The main class creates an instance of the data class through its constructor. The data class exposes each attribute through a getter, and through a setter only if the attribute must change later.[^1]

```text
 Order                                 OrderData (read-only)
 -----                                 ---------------------
 - data: OrderData  ----------------->  order_id   (getter only)
 - items: list      (still mutable)     customer   (getter only)
 + add(item)
 + describe()  -- reads data.order_id, data.customer
```

The pattern lets the designer remove the write privilege of attributes that are meant to be set only during construction, even from methods of the target class.[^1] It also reduces the number of fields of the class, because the protected fields become one object.[^1]

Here the same bug fails at once, because the data object is frozen:[^2]

```python run id=frozen-data
from dataclasses import dataclass, FrozenInstanceError


@dataclass(frozen=True)
class OrderData:
    order_id: int
    customer: str


class Order:
    def __init__(self, order_id, customer):
        self._data = OrderData(order_id, customer)
        self.items = []

    def merge(self, other):
        self.items += other.items
        self._data.order_id = other._data.order_id  # the same bug


a = Order(42, "ann")
b = Order(43, "bob")
try:
    a.merge(b)
except FrozenInstanceError as e:
    print("merge failed:", e)
print("order", a._data.order_id, "for", a._data.customer)
```

```output for=frozen-data
merge failed: cannot assign to field 'order_id'
order 42 for ann
```

## Example

The example files show one `Order` in each language. Its id and customer are in a read-only data object, and its list of items stays in the class. The program adds one item to an order with two items, and prints:

```text
order 42 for ann: 2 items
order 42 for ann: 3 items
```

| Language | Data object | Read-only by |
|---|---|---|
| Python | a frozen `dataclass` | an exception on assignment[^2] |
| JavaScript | a frozen object in a private `#data` field | `Object.freeze` and the private field |
| TypeScript | a class with `readonly` properties | the compiler |
| Java | a `record` | component fields that are private and final[^3] |
| C# | a positional `record` | `init` accessors, which assign only during object construction[^4] |
| C++ | a `struct` held as a `const` member | the compiler |
| Go | a struct with unexported fields | package privacy |
| Rust | a struct with private fields in a module | privacy: fields of a public struct stay private[^5] |

## Consequences

The benefits:

- Fields that must not change cannot change, even through a bug in the class itself.
- The class shows in one place which state is fixed and which state changes.
- The data object can be passed, compared and copied as a value.

The costs:

- **One more class.** Each protected class gets a data class, and each access goes through it.
- **Shallow protection.** A read-only reference to a mutable object does not protect the object. A frozen data object that holds a list still lets callers change the list.
- **Little gain with modern types.** Where the language has final fields, records or read-only properties, the language already gives the protection, and the extra object adds only indirection.

## When not to use it

Do not use the pattern when the language can mark the field itself as final or read-only. A Java `final` field, a C# `readonly` field or a property with an `init` accessor gives the same protection with no extra class.[^4]

Do not use the pattern for a class whose fields all change. There is nothing to protect, and the data object only adds a level of access.

## Variants and related patterns

**Immutable value objects.** A data object with no setters is an immutable value. Java records, C# records and Python frozen dataclasses make such an object in one declaration.[^3][^4][^2] See [[immutability]].

**Indirection for safety.** The Go `os` package stores the data of a `File` in a separate, unexported struct, so that clients cannot overwrite it.[^6][^7]

## Known uses

- Go: [os/types.go at go1.25.0](https://github.com/golang/go/blob/go1.25.0/src/os/types.go#L14-L20) defines `File` as a struct that only embeds a pointer to the unexported type `file`. [os/file_unix.go](https://github.com/golang/go/blob/go1.25.0/src/os/file_unix.go#L55-L66) says that the extra level of indirection ensures that no clients of `os` can overwrite this data, which could cause the finalizer to close the wrong file descriptor.[^6][^7]

## References
