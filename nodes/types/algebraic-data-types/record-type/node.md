---
tag: TGG8Y0
kind: concept
slug: record-type
name: Record type
aliases: [record, record type, struct, structure, data class, named fields, compound data type, user-defined type]
area: types/algebraic-data-types
level: intro
summary: >-
  A record groups a fixed set of named fields, each with its own type. Programs read a field by name and often make an updated copy in place of a change.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1966
  people: [Niklaus Wirth, Tony Hoare]
  note: COBOL was the first widespread language with record types. ALGOL W, from a proposal by Wirth and Hoare, added records to the ALGOL family, and Pascal, which ALGOL W influenced, made records part of its type system.
see_also: [product-type, algebraic-data-type, sum-types, type, pattern-matching, subtyping]
same_as:
  wikidata: Q2470517
  wikipedia: Record_(computer_science)
sources:
  - url: https://en.wikipedia.org/wiki/Record_(computer_science)
    title: Record (computer science), Wikipedia
    locator: the introduction; History
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch05-01-defining-structs.html
    title: The Rust Programming Language, 5.1 Defining and Instantiating Structs
    locator: the introduction; Creating Instances with Struct Update Syntax
    accessed: 2026-11-16
  - url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/ClassesAndStructures.md
    title: The Swift Programming Language, Structures and Classes
    locator: Memberwise Initializers for Structure Types; Structures and Enumerations Are Value Types
    accessed: 2026-11-16
  - url: https://ocaml.org/manual/5.3/expr.html
    title: The OCaml Manual, release 5.3, Expressions
    locator: 11.7.2 Records (the expression with)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/records
    title: F# language reference, Records
    locator: Remarks; Creating Records by Using Record Expressions
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/with-expression
    title: C# language reference, with expression
    locator: the introduction
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Kotlin documentation, Data classes
    locator: the introduction (the generated members); Copying
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dataclasses.html
    title: The Python Standard Library, dataclasses
    locator: the introduction; replace(); frozen instances
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax
    title: MDN Web Docs, Spread syntax (...)
    locator: Description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/records
    title: C# fundamentals, Record types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://openjdk.org/jeps/395
    title: "JEP 395: Records"
    locator: the header (Release 16); Summary; History
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html#readonly-properties
    title: TypeScript Handbook, Object Types
    locator: readonly Properties
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause; Comparison operators
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/ALGOL_W
    title: ALGOL W, Wikipedia
    locator: the infobox; the introduction
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0557/
    title: "PEP 557: Data Classes"
    locator: the header (Python-Version 3.7); Abstract
    accessed: 2026-11-16
---

## Idea

A record is a group of values with names. A date has a year, a month and a day. A person has a name and an age. Wikipedia defines a record as "a collection of fields, possibly of different data types, typically fixed in number and sequence".[^1] Each field has a name and a type, and the type of the record lists them.

```python run id=idea
from dataclasses import dataclass, replace


@dataclass(frozen=True)
class Person:
    name: str
    age: int


ada = Person(name="Ada", age=36)
print(ada.name, ada.age)

older = replace(ada, age=ada.age + 1)
print(older)
print(ada)
```

```output for=idea
Ada 36
Person(name='Ada', age=37)
Person(name='Ada', age=36)
```

A record is a product type whose parts have names (see [[product-type]]). The names make the code clear. Rust says it: "Unlike with tuples, in a struct you'll name each piece of data so it's clear what the values mean."[^2] The names also free the code from the order of the fields: "You don't have to rely on the order of the data to specify or access the values of an instance."[^2]

## How it works

**Define, build and read.** A record type declares its fields. A value is built with one value for each field, usually by name, as in the call `Person(name=n, age=a)`. A field is read with a dot, as in `ada.name`. Swift structures get "an automatically generated *memberwise initializer*", whose arguments are the fields by name.[^3]

**Update by copy.** Many records are immutable. To change one field, the program makes a new record that has the new value in that field and the old values in the other fields. Most languages have a short form for this:

| Language | Copy with a change |
|---|---|
| OCaml | `{ p with age = 37 }` builds "a fresh record" with the other fields from `p`[^4] |
| F# | `{ p with Age = 37 }`, a copy and update record expression[^5] |
| Rust | `Person { age: 37, ..p }`, the struct update syntax[^2] |
| C# | `p with { Age = 37 }`, which "creates a copy of its operand with the specified properties and fields modified"[^6] |
| Kotlin | `p.copy(age = 37)`[^7] |
| Python | `dataclasses.replace(p, age=37)`[^8] |
| JavaScript, TypeScript | `{ ...p, age: 37 }`, with spread syntax[^9] |

The copy is shallow. OCaml says that the `with` expression "returns a shallow copy of the record expr, except for the fields field1 … fieldn".[^4] So a field that holds a mutable object is shared between the old and the new record. The Pitfalls show the result.

**Generated methods.** A record usually comes with methods that the compiler writes. The C# `record` modifier makes the compiler "generate value equality", a formatted `ToString`, and "nondestructive mutation through with expressions".[^10] A Kotlin data class gets `equals()`, `hashCode()`, `toString()`, `componentN()` functions and `copy()`.[^7] A Python dataclass gets generated methods "such as __init__() and __repr__()".[^8] Java records are "classes that act as transparent carriers for immutable data".[^11]

**Values and references.** In some languages a record is a value. Assignment copies all the fields. In Swift, a value type "is a type whose value is *copied* when it's assigned to a variable or constant, or when it's passed to a function".[^3] C structs, Go structs, Rust structs and C# `record struct` types behave this way. In other languages a record is an object, and assignment copies a reference. Java records, C# `record class` types, Kotlin data classes and Python dataclasses behave this way. With a reference, a change through one name is visible through all names, so records of this kind are often immutable.

**Nominal and structural.** In most languages, two record types with the same fields are still two different types, because the type has a name. Java calls records "nominal tuples".[^11] TypeScript compares object types by their fields, so any object with the right fields has the type. OCaml finds the record type from the field names that an expression uses.

## In languages

The example files define a `Person` record, read a field, make a copy with a new age, and compare two records.

| Language | Verdict | Form |
|---|---|---|
| Python | native | a `@dataclass`, frozen for an immutable record[^8] |
| JavaScript | idiom | an object literal; `Object.freeze` for an immutable record |
| TypeScript | native | an object type with `readonly` fields[^12] |
| Java | native | a `record`[^11] |
| C | native | a `struct` |
| C++ | native | a `struct`, an aggregate |
| C# | native | a `record`, a `record struct` or a `struct`[^10] |
| Go | native | a `struct` |
| Rust | native | a `struct`[^2] |
| Kotlin | native | a `data class`[^7] |
| PowerShell | idiom | a `[pscustomobject]` or a class |
| Bash | idiom | an associative array with one key for each field |
| SQL | native | a row of a table, with one column for each field |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types |

**Python.** `@dataclass(frozen=True)` makes the fields read-only after construction. `dataclasses.replace` "Creates a new object of the same type as obj, replacing fields with values from changes".[^8]

**JavaScript and TypeScript.** An object is a record of properties. In an object literal, the spread syntax "enumerates the properties of an object and adds the key-value pairs to the object being created".[^9] In TypeScript, the type checker refuses a write to a `readonly` property, but the mark does not change any behavior at run time.[^12]

**Java.** For each component, a record class gets a private final field and a public accessor method with the same name. It also gets a canonical constructor and implicit `equals`, `hashCode` and `toString` methods. Java 16 made records a standard feature.[^11] Java has no short form for a copy with a change, so code calls the constructor with all the fields.

**C, C++ and Go.** A struct is a value. Assignment copies every field, so a copy with a change is an assignment followed by a write to one field. C has no `==` for structs, so a program compares the fields one by one. In Go, "Struct types are comparable if all their field types are comparable", and `==` compares them field by field.[^13]

## Variations and related ideas

- **Tuples.** A product whose parts have positions, not names. Java calls records "nominal tuples".[^11] See [[tuple]].
- **Sums of records.** A sum type whose cases are records, such as a sealed interface with record cases in Java. See [[sum-types]] and [[algebraic-data-type]].
- **Classes.** A class can hold fields like a record, but it usually hides them behind methods. A record shows its fields.
- **Row polymorphism.** A function can accept any record that has at least some fields. See [[row-polymorphism]].
- **Database rows.** A row of a table is a record with one field for each column.

## Pitfalls

### A shallow copy shares a mutable field

A copy with a change copies the fields, not the objects that the fields refer to. Here `team_b` gets a new name, but its list of members is the same list as in `team_a`.

```python wrong id=shallow
from dataclasses import dataclass, replace


@dataclass(frozen=True)
class Team:
    name: str
    members: list[str]


team_a = Team("A", ["Ada"])
team_b = replace(team_a, name="B")
team_b.members.append("Grace")
print(team_a.members)
```

```output for=shallow
['Ada', 'Grace']
```

`frozen=True` stops assignment to the fields, but it does not stop a change inside the list. The fix is an immutable field type, such as `tuple[str, ...]`, or a copy of the list: `replace(team_a, name="B", members=list(team_a.members))`.

### Changing a copy of a struct in a loop

In Go, a `range` loop assigns each element to the loop variable "as in an assignment statement".[^13] A struct assignment copies the struct, so a change to the loop variable changes only the copy.

```go wrong id=range-copy
package main

import "fmt"

type Person struct {
	Name string
	Age  int
}

func main() {
	people := []Person{{"Ada", 36}, {"Grace", 45}}
	for _, p := range people {
		p.Age++
	}
	fmt.Println(people)
}
```

```output for=range-copy
[{Ada 36} {Grace 45}]
```

The ages did not change. The fix is to write through the index: `for i := range people { people[i].Age++ }`.

## Cost

A record stores each field once, plus any padding. Reading a field is a load at a fixed offset. A copy with a change copies every field once, so its cost grows with the number of fields, not with the size of the objects that the fields refer to. A record that is a value type is copied in full on each assignment and each call. A large struct that is passed often can be passed by pointer or by reference.

## History

"COBOL was the first widespread programming language to support record types."[^1] Its records could nest, and its `MOVE CORRESPONDING` statement copied fields between two records by name.[^1] ALGOL W, designed by Niklaus Wirth and Tony Hoare, added "reference to record data types" to ALGOL 60.[^14] "The Pascal programming language was one of the first languages to fully integrate record types with other basic types into a logically consistent type system."[^1] C provides records as structs.[^1]

Later languages added records that the compiler completes with methods. Python 3.7 added data classes with PEP 557.[^15] Java records were a preview in JDK 14 and became final in JDK 16.[^11] C# added the `record` modifier, and Kotlin has data classes.[^10][^7]

## References
