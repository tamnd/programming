---
tag: Q32DVX
kind: concept
slug: data-classes
name: Data classes
aliases: [dataclass, data carrier class, case class, record class, record struct, data-only class, generated value methods]
area: objects-and-classes/standard-object-methods
level: core
summary: A data class is a class made to hold a fixed set of values, for which the compiler generates the constructor, equality, the hash, the string form and often a copy method.
applies_to: core-languages
viz: optional
history:
  year: 2017
  note: PEP 557, created in 2017, added dataclasses to Python 3.7; Scala case classes and Kotlin data classes came before it, and JEP 395 made Java records final in JDK 16.
see_also: [record-type, define-a-record-type, universal-object-methods, implement-equality-for-a-type, object-cloning, immutability, string-representation]
sources:
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Data classes, Kotlin documentation
    locator: Introduction; Properties declared in the class body; Copying
  - url: https://openjdk.org/jeps/395
    title: "JEP 395: Records"
    locator: Summary; Goals; History
  - url: https://docs.python.org/3/library/dataclasses.html
    title: dataclasses, Data Classes, Python standard library
    locator: Module contents; replace
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Record.html
    title: Record (Java SE 21 & JDK 21), API documentation
    locator: Description
  - url: https://peps.python.org/pep-0557/
    title: "PEP 557: Data Classes"
    locator: Header; Abstract
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record
    title: Records, C# reference
    locator: Introduction; Immutability; Built-in formatting for display
  - url: https://docs.scala-lang.org/tour/case-classes.html
    title: Case Classes, Tour of Scala
    locator: Comparison; Copying
  - url: https://doc.rust-lang.org/book/appendix-03-derivable-traits.html
    title: "Appendix C: Derivable Traits, The Rust Programming Language"
    locator: Introduction; PartialEq and Eq; Hash
  - url: https://en.cppreference.com/w/cpp/language/default_comparisons
    title: Default comparisons, cppreference.com
    locator: Introduction
---

## Idea

Many classes exist only to hold values: a point with `x` and `y`, a book with a title and a year, a message with a sender and a text. Such a class needs the same methods each time: a constructor that sets each field, equality that compares the fields, a hash that agrees with equality, a string form that shows the fields, and often a way to copy the object with one field changed. Written by hand, these methods are long, and each new field must be added to all of them.

A data class asks the compiler to write the methods from the list of fields. Kotlin: "For each data class, the compiler automatically generates additional member functions that allow you to print an instance to readable output, compare instances, copy instances, and more."[^1] Java's JEP 395 gives the same goal: "Automatically implement data-driven methods such as equals and accessors."[^2]

A data class is a [record type](/record-type/) with methods. The record type is the idea of a value with named fields; the data class is the language feature that gives such a value the [universal object methods](/universal-object-methods/) for free.

## How it works

The programmer lists the fields, and the compiler or a library generates the methods from that list. Python's `@dataclass` reads the annotated class variables and adds "generated special methods such as __init__() and __repr__()".[^3] With `frozen=True`, the fields cannot change, and the class also gets a hash:

```python run id=generated-methods
from dataclasses import dataclass, replace


@dataclass(frozen=True)
class Book:
    title: str
    year: int
    tags: tuple = ()


a = Book("Dune", 1965)
b = Book("Dune", 1965)
print(a)
print(a == b, a is b, hash(a) == hash(b))
c = replace(a, year=1966)
print(c)
try:
    a.year = 2000
except AttributeError as e:
    print(type(e).__name__)
```

```output for=generated-methods
Book(title='Dune', year=1965, tags=())
True False True
Book(title='Dune', year=1966, tags=())
FrozenInstanceError
```

`a` and `b` are two objects, but they are equal and have equal hashes, because the generated methods compare the fields. `replace` makes a new object with one field changed, so a frozen object is never changed in place.

The generated methods are the same in each language:

| Method | Python `@dataclass` | Java `record` | Kotlin `data class` | C# `record` | Rust `#[derive]` |
|---|---|---|---|---|---|
| constructor | `__init__` | canonical constructor | primary constructor | primary constructor | struct literal |
| equality | `__eq__` | `equals` | `equals` | `Equals`, `==` | `PartialEq`, `Eq` |
| hash | `__hash__` when frozen | `hashCode` | `hashCode` | `GetHashCode` | `Hash` |
| string form | `__repr__` | `toString` | `toString` | `ToString` | `Debug` |
| copy with changes | `replace` | none in Java 21 | `copy` | `with` | struct update syntax |
| order | `order=True` | none | none | none | `PartialOrd`, `Ord` |

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `@dataclass`; also `typing.NamedTuple` |
| JavaScript | absent | no generated methods; plain objects and classes |
| TypeScript | absent | as JavaScript; type aliases and interfaces describe the shape only |
| Java | native | `record` classes |
| Kotlin | native | `data class` |
| C# | native | `record`, `record class` and `record struct` |
| Go | idiom | plain structs: `==` and `%+v` work on them without code |
| Rust | stdlib | `#[derive(Debug, Clone, PartialEq, Eq, Hash)]` on a struct |
| C | absent | plain structs without methods |
| C++ | idiom | aggregates with defaulted `operator==` and `operator<=>` |
| Bash | n/a | no user types |
| PowerShell | idiom | `[pscustomobject]` holds named values; no generated equality |
| SQL | n/a | rows of a table hold the data; no classes |
| Regex | n/a | no user types |
| HTML | n/a | no user types |
| CSS | n/a | no user types |

- **Kotlin** derives the methods "from all properties declared in the primary constructor": the `equals()` and `hashCode()` pair, a `toString()`, `componentN()` functions and `copy()`.[^1] A property declared in the class body is not part of them.
- **Java** records are "classes that act as transparent carriers for immutable data".[^2] The fields are final, and `equals`, `hashCode` and `toString` "are derived from all of the component fields".[^4] "A record class is implicitly final, and cannot be abstract."[^2]
- **Python** dataclasses are normal classes with generated methods. PEP 557 says that they "can be thought of as “mutable namedtuples with defaults”".[^5] A hash comes only with `frozen=True` or `unsafe_hash=True`.
- **C#** records give "Value equality", "Concise syntax for nondestructive mutation" and "Built-in formatting for display".[^6] A `record` is a reference type and a `record struct` is a value type.
- **Scala** case classes are "good for modeling immutable data", and they are "compared by structure and not by reference".[^7]
- **Rust** has no data class keyword. `#[derive]` writes each trait from the fields: "When PartialEq is derived on structs, two instances are equal only if all fields are equal".[^8]
- **C++** lets a class ask for a generated comparison: "Comparison operator functions can be explicitly defaulted to request the compiler to generate the corresponding default comparison for a class."[^9]

## Variations and related ideas

- **Tuples with names.** Python's `namedtuple` and `typing.NamedTuple` give a tuple named fields, and JEP 395 says that "Records can be thought of as nominal tuples".[^2] A tuple is ordered and can be indexed; a data class has only names.
- **Copy with changes.** Kotlin's `copy`, C#'s `with`, Scala's `copy` and Python's `replace` make a new object with some fields changed. The copy is shallow: see [object cloning](/object-cloning/).
- **Validation.** A data class still has a constructor that can check its values. Java records have a compact constructor, Python dataclasses have `__post_init__`, and Kotlin has `init` blocks.
- **Pattern matching.** The fixed list of fields makes data classes good targets for destructuring and `match`: Kotlin's `componentN`, Python's `__match_args__`, and Java's record patterns.
- **Immutability.** Most data classes are immutable or can be made so: Java records, Scala case classes and frozen dataclasses. See [immutability](/immutability/).

## Pitfalls

- **Equality without a hash.** A mutable Python dataclass has `__eq__`, but its hash is set to `None`: "If eq is true and frozen is false, __hash__() will be set to None, marking it unhashable (which it is, since it is mutable)."[^3] Its objects cannot go in a set:

```python run id=mutable-dataclass-is-unhashable
from dataclasses import dataclass


@dataclass
class Point:
    x: int
    y: int


p = Point(1, 2)
print(p == Point(1, 2))
try:
    seen = {p}
except TypeError:
    print("TypeError: Point is unhashable")
```

```output for=mutable-dataclass-is-unhashable
True
TypeError: Point is unhashable
```

  Use `frozen=True` for values that go in sets or are dictionary keys.
- **Shallow immutability.** A Java record is "a shallowly immutable, transparent carrier for a fixed set of values".[^4] A record that holds a list can still have the list changed. C# says the same of its records: "the data that a reference-type property refers to can be changed".[^6] Copy mutable inputs in the constructor, or store immutable collections.
- **Fields left out of equality.** A Kotlin property declared in the class body is not in `equals`, so two objects that differ in it are equal.[^1] Put every field that matters in the primary constructor.
- **Equality and inheritance.** Generated equality compares the fields of one class. A subclass that adds a field cannot keep `equals` symmetric with its parent. Java records are final,[^2] and Kotlin data classes "can't be abstract, open, sealed, or inner",[^1] so no subclass can add a field to them. See [implement equality for a type](/implement-equality-for-a-type/).
- **Secrets in the string form.** The generated `toString` prints every field, including a password or a token. Override it for such types.

## Cost

A data class costs the same as a hand-written class with the same methods. The generated equality and hash read every field, so their cost grows with the number and the size of the fields; a data class with a large list field compares and hashes the whole list. A copy with changes allocates a new object each time, which matters only in hot loops.

## History

Scala has case classes and Kotlin has data classes. Python got dataclasses from PEP 557, created on June 2, 2017, for Python 3.7.[^5] Java records were a preview in JDK 14 and JDK 15, and JEP 395 made them final in JDK 16.[^2] C# added records, and Rust has derived the same methods from struct fields with `#[derive]`.

## References
