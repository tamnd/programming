---
tag: ET4237
kind: concept
slug: algebraic-data-type
name: Algebraic data type
aliases: [algebraic data type, algebraic datatype, algebraic type, sum of products, data declaration]
area: types/algebraic-data-types
level: core
summary: >-
  An algebraic data type is built from other types with two operations. A product holds one value of each part, and a sum holds one value of one of several cases.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1977
  people: [Rod Burstall, David MacQueen, Don Sannella]
  note: Algebraic types first appeared in the language NPL of Burstall in 1977, and in Hope by Burstall, MacQueen and Sannella in 1980. Haskell took the sum of products form from Miranda.
see_also: [sum-types, pattern-matching, type, type-system, null-references, parametric-polymorphism, subtyping, hindley-milner]
same_as:
  wikidata: Q1322511
  wikipedia: Algebraic_data_type
sources:
  - url: https://en.wikipedia.org/wiki/Algebraic_data_type
    title: Algebraic data type, Wikipedia
    locator: the introduction; Theory
    accessed: 2026-11-16
  - url: https://www.microsoft.com/en-us/research/wp-content/uploads/2016/07/history.pdf
    title: P. Hudak, J. Hughes, S. Peyton Jones and P. Wadler, A History of Haskell, Being Lazy With Class, HOPL III, 2007
    locator: 2.2 (the Lisp conference of August 1980); 5 Data types and pattern matching; 5.1 Algebraic types
    accessed: 2026-11-16
  - url: https://codewords.recurse.com/issues/three/algebra-and-calculus-of-algebraic-data-types
    title: Joel Burget, The algebra (and calculus!) of algebraic data types, Code Words, issue 3
    locator: Counting inhabitants
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Unit_type
    title: Unit type, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.2.1 Algebraic Datatype Declarations
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.2.5p20
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.2.5p20 (structure and union types); 6.7.2.1p15 and p16 (padding; the size of a union)
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dataclasses.html
    title: The Python Standard Library, dataclasses
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/typing.html#typing.Union
    title: The Python Standard Library, typing
    locator: typing.Union
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions
    accessed: 2026-11-16
  - url: https://openjdk.org/jeps/409
    title: "JEP 409: Sealed Classes"
    locator: Sealing and record classes
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/tuple.general
    title: Working Draft, Standard for Programming Language C++ (N4950), [tuple.general]
    locator: 22.4.1 In general
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/variant.variant.general
    title: Working Draft, Standard for Programming Language C++ (N4950), [variant.variant.general]
    locator: 22.6.3.1 General
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/records
    title: C# fundamentals, Record types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#variant_types
    title: Go FAQ, Why does Go not have variant types?
    locator: the answer
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html
    title: The Rust Programming Language, 6.1 Defining an Enum
    locator: the introduction
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/sealed-classes.html
    title: Kotlin documentation, Sealed classes and interfaces
    locator: the introduction
    accessed: 2026-11-16
  - url: https://ocaml.org/manual/5.3/typedecl.html
    title: The OCaml Manual, release 5.3, Type and exception definitions
    locator: Type definitions
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/discriminated-unions
    title: F# language reference, Discriminated Unions
    locator: Remarks
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/records
    title: F# language reference, Records
    locator: Remarks
    accessed: 2026-11-16
  - url: https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/Enumerations.md
    title: The Swift Programming Language, Enumerations
    locator: the introduction; Associated Values
    accessed: 2026-11-16
  - url: https://docs.scala-lang.org/scala3/reference/enums/adts.html
    title: Scala 3 Reference, Algebraic Data Types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Abstract_data_type
    title: Abstract data type, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
---

## Idea

Many values are built from other values in two ways. Some values have several parts at the same time. A point has an x coordinate and a y coordinate. Other values have one of several forms. A payment is cash, or a card, or a voucher. An algebraic data type is a type built with these two operations, *and* and *or*. Wikipedia names them: "An algebraic data type is defined by two key constructions: a sum and a product."[^1]

- A **product** holds one value of each of its parts. A record, a struct and a tuple are products.
- A **sum** holds one value of one of a fixed list of cases. Each case has a name, the constructor, and each case can carry its own data. [[sum-types]] describes sums in detail.

Most useful types mix the two. A payment is a sum of three cases, and each case is a product of its fields. Haskell defines its algebraic types in this form, as a sum of products.[^2]

```python run id=idea
from dataclasses import dataclass


@dataclass
class Cash:
    amount: int


@dataclass
class Card:
    last4: str
    amount: int


@dataclass
class Voucher:
    code: str


# A sum of three cases. Each case is a product of its fields.
type Payment = Cash | Card | Voucher


def describe(p: Payment) -> str:
    match p:
        case Cash(amount):
            return f"cash {amount}"
        case Card(last4, amount):
            return f"card ending {last4}, {amount}"
        case Voucher(code):
            return f"voucher {code}"


for p in [Cash(20), Card("4242", 35), Voucher("SPRING")]:
    print(describe(p))
```

```output for=idea
cash 20
card ending 4242, 35
voucher SPRING
```

A program reads a value of an algebraic data type with pattern matching. The match finds the case, and it gives names to the data of that case.[^1] See [[pattern-matching]].

## How it works

**Why the word algebraic.** The names come from counting. Count the values of each type. A product of `A` and `B` has one value for each combination of a value of `A` and a value of `B`, so it has `|A| × |B|` values. A sum of `A` and `B` has the values of `A` and the values of `B`, so it has `|A| + |B|` values.[^3] The examples on this page check the numbers with a flag of 2 values and a traffic light of 3 values. The pair (flag, light) has 2 × 3 = 6 values. The choice of a flag or a light has 2 + 3 = 5 values.

**The empty product and the empty sum.** A product with no parts has exactly one value. This is the unit type, "the type of 0-tuples, i.e. the product of no types".[^4] A sum with no cases has no values. This is the empty type, which "allows no values".[^4] So the unit type acts as the number 1, and the empty type acts as the number 0. A boolean is the sum of two units, `1 + 1 = 2`.[^3]

**Constructors.** Each case of a sum has a constructor. A constructor takes the fields of its case and gives a value of the whole type. In Haskell, a data declaration "introduces a new type constructor T with zero or more constituent data constructors".[^5] A constructor with no fields is a constant, such as `Nothing` or `Red`. A sum whose constructors all have no fields is an enumeration.[^1]

**Type parameters.** An algebraic data type can take type parameters. Haskell's `Maybe a` is `Nothing | Just a`, and it works for any type `a`.[^2] See [[parametric-polymorphism]].

**Recursion.** A case can contain a value of the type that it belongs to. A list is empty, or it is a head and a tail list. A tree is a leaf, or a branch with two subtrees. The History of Haskell gives this tree: `data Tree a = Leaf a | Branch (Tree a) (Tree a)`.[^2] In type theory, the list type is written `λα.μβ.1+α×β`. The empty list is the 1, and a cell is the product of an element and a list.[^1] See [[recursive-type]].

**Matching.** A value of a sum records its case. A `match`, a `case` or a `switch` reads the case and binds the fields. The fields of a case exist only inside the branch of that case, so the program cannot read a field of the wrong case. The list of cases is closed, so the compiler can check that a match handles every case.[^1] [[sum-types]] shows this check in each language.

**Products and sums in memory.** A product stores all of its fields, so its size is at least the sum of the sizes of its fields. A C structure can also have "unnamed padding" between members.[^6] A sum stores one case and a tag that tells which case it is. A C union has no tag, and its size "is sufficient to contain the largest of its members".[^6] So a tagged sum needs the tag plus the largest case.

## In languages

The example files show the counting program in each language: a product of a flag and a light, and a sum of a flag or a light. The table gives the form of products and sums in each language.

| Language | Verdict | Product | Sum |
|---|---|---|---|
| Python | idiom | a `dataclass`[^7] or a tuple | a union `A \| B` of classes, checked by a type checker only[^8] |
| JavaScript | idiom | an object | an object with a `kind` field |
| TypeScript | native | an object type or a tuple type | a discriminated union[^9] |
| Java | native | a `record` | a `sealed` interface with `record` cases[^10] |
| C | idiom | a `struct`[^6] | a `struct` with an `enum` tag and a `union`[^6] |
| C++ | stdlib | a `struct`, `std::pair` or `std::tuple`[^11] | `std::variant`[^12] |
| C# | idiom | a `record`[^13] | an abstract record with sealed records |
| Go | idiom | a `struct` | an interface with a closed set of types, read with a type switch[^14] |
| Rust | native | a `struct` or a tuple | an `enum`[^15] |
| Kotlin | native | a `data class` | a `sealed` class or interface[^16] |
| PowerShell | absent | a class or a hashtable | none |
| Bash | n/a | no types | no types |
| SQL | idiom | a row of a table | a kind column, or one table for each case |
| Regex | n/a | a pattern has no types | a pattern has no types |
| HTML/CSS | n/a | markup has no types | markup has no types |

**ML, Haskell, OCaml and F#.** In these languages, one declaration defines a sum of products. Haskell 2010 calls it an "algebraic datatype declaration".[^5] OCaml type definitions bind type constructors to "variant types, record types, type abbreviations, or abstract data types".[^17] F# has discriminated unions, which "provide support for values that can be one of a number of named cases, possibly each with different values and types".[^18] F# records "represent simple aggregates of named values".[^19]

**Rust and Swift.** Both languages use the word `enum` for a sum, and a `struct` for a product. The Rust book says that "enums give you a way of saying a value is one of a possible set of values".[^15] In Swift, enumeration cases "can specify associated values of *any* type to be stored along with each different case value, much as unions or variants do in other languages".[^20]

**Scala.** In Scala 3, "The enum concept is general enough to also support algebraic data types (ADTs)".[^21] Case classes are the products.

**Java and Kotlin.** Java builds both operations from classes. JEP 409 says: "Record classes allow us to express product types, and sealed classes allow us to express sum types."[^10] In Kotlin, "All direct subclasses of a sealed class are known at compile time."[^16] So a sealed class with data classes as its subclasses is a sum of products.

**C#.** The `record` modifier tells the compiler to "generate value equality", a formatted `ToString` and "nondestructive mutation through with expressions".[^13] A record is a product. The C# of the examples has no closed sum, so an abstract record with sealed records stands for one. Other code can add a case to it, so the compiler cannot check that a `switch` over it is complete. [[sum-types]] describes the `union` and `closed` types of C# 15.

**Go.** Go has structs, but no sums. The Go FAQ says that variant types "overlap in confusing ways with interfaces". An interface value with a type switch takes their place.[^14]

**C and C++.** A C structure "describes a sequentially allocated nonempty set of member objects", and a C union "describes an overlapping nonempty set of member objects".[^6] A union does not record which member it holds, so a C program keeps a separate tag. In C++, `std::tuple` gives "heterogeneous, fixed-size collections of values".[^11] `std::variant` holds a value of one of its alternative types, and it knows which one.[^12]

## Variations and related ideas

- **Product types.** Records, structs and tuples. See [[product-type]], [[record-type]] and [[tuple]].
- **Sum types.** Tagged unions, variants and sealed hierarchies. See [[sum-types]].
- **Enumerations.** A sum whose cases carry no data. See [[enumeration]].
- **Option and result types.** `Maybe a` in Haskell and `Option<T>` in Rust are sums with a case for no value. They replace the null reference. See [[null-references]].
- **Generalized algebraic data types.** A GADT lets each constructor give a more specific result type. Scala 3 enums support "algebraic data types (ADTs) and their generalized version (GADTs)".[^21] See [[generalized-algebraic-data-type]].
- **Row polymorphism and open variants.** Some languages let a record or a variant have more fields or more cases than a function names. See [[row-polymorphism]].
- **Untagged unions.** A union type with no tag, such as `int | str` in Python, is not the same as a sum. See [[union-type]].

## Pitfalls

### An abstract data type is not an algebraic data type

Both names are shortened to ADT, but they are different ideas. An abstract data type is "defined by its behavior (semantics) from the point of view of a user of the data".[^22] A stack is defined by `push` and `pop`, and its representation is hidden. An algebraic data type is the opposite. Its representation is the definition: the cases and the fields are public, and the code matches on them. The Wikipedia article on algebraic data types starts with the note "Not to be confused with Abstract data type."[^1] See [[abstract-data-type]].

### A product where a sum is needed

A record with a flag for each state is a product, so it can hold every combination of the flags. Some of the combinations have no meaning. Here a request has a flag `loading`, an optional `error` and an optional `data`. That is 2 × 2 × 2 = 8 combinations of set and not set, but a request has only 3 real states.

```python wrong id=flags
from dataclasses import dataclass


@dataclass
class Request:
    loading: bool
    error: str | None
    data: str | None


r = Request(loading=True, error="timeout", data="partial")
print("loading and failed at the same time:", r.loading and r.error is not None)
```

```output for=flags
loading and failed at the same time: True
```

The constructor accepts this value, and each reader must decide what it means. The fix is a sum of three cases, `Loading`, `Failed(error)` and `Loaded(data)`. The sum has exactly the 3 legal states. See [[make-illegal-states-unrepresentable]].

### A sum that a language cannot close

In Python, TypeScript and JavaScript, the cases of a union exist only in the type checker, or only in the program's convention. In C# and Go, any code can add one more class or one more type that implements the interface. In these languages, a new case does not stop the build. [[sum-types]] shows the missing case in each language, and how to make it fail early.

## Cost

A value of a product has all of its fields, and a value of a sum has one case and a tag. Construction allocates the value once. A match reads the tag and selects a branch, which is usually a jump through a table or a short chain of tests. In C, the size of a union is the size of its largest member,[^6] so a sum of a small case and a large case uses the large size for every value. Languages that store each case as an object on the heap, such as Java, C#, Kotlin and Python, add an allocation and a reference for each value.

## History

Algebraic types "first appeared in Burstall's NPL (Burstall, 1977) and Burstall, MacQueen, and Sannella's Hope (Burstall et al., 1980)".[^2] At the first Lisp conference, in August 1980, Burstall, MacQueen and Sannella presented "Hope, the language that introduced algebraic data types".[^2] Algebraic types "were absent from the original ML (Gordon et al., 1979) and KRC (Turner, 1982), but appeared in their successors Standard ML (Milner et al., 1997) and Miranda (Turner, 1986)".[^2] Hope and Standard ML kept sums and products apart: a branch of a tree took one argument, a tuple of two trees. Haskell took the sum of products form from Miranda, so a constructor takes several fields.[^2]

Mainstream languages added the two parts later. Java added records in Java 16 and sealed classes in Java 17.[^1] JEP 409 says that the combination "is sometimes referred to as algebraic data types".[^10] Rust (with `enum`), Swift, Kotlin, Scala and TypeScript all have the two operations.[^15][^20][^16][^21][^9]

## References
