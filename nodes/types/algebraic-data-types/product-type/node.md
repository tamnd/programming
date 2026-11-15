---
tag: PWHRX6
kind: concept
slug: product-type
name: Product type
aliases: [product type, product, cartesian product type, pair type, struct type, composite type, unit type]
area: types/algebraic-data-types
level: core
summary: >-
  A value of a product type holds one value of each of its parts at the same time. Records, structs and tuples are product types, and the unit type is the product with no parts.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1966
  people: [Niklaus Wirth, Tony Hoare]
  note: COBOL was the first widespread language with record types. ALGOL W, from a proposal by Wirth and Hoare, added records to the ALGOL family in 1966, and Pascal took them from ALGOL W.
see_also: [algebraic-data-type, sum-types, type, type-system, pattern-matching, parametric-polymorphism]
same_as:
  wikidata: Q7247800
  wikipedia: Product_type
sources:
  - url: https://en.wikipedia.org/wiki/Algebraic_data_type
    title: Algebraic data type, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Product_type
    title: Product type, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch05-01-defining-structs.html
    title: The Rust Programming Language, 5.1 Defining and Instantiating Structs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://codewords.recurse.com/issues/three/algebra-and-calculus-of-algebraic-data-types
    title: Joel Burget, The algebra (and calculus!) of algebraic data types, Code Words, issue 3
    locator: Counting inhabitants
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Unit_type
    title: Unit type, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html#the-tuple-type
    title: The Rust Programming Language, 3.2 Data Types
    locator: The Tuple Type
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Comparison operators (struct types); Size and alignment guarantees
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.10.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.10.3 Record Members (the implicitly declared equals method)
    accessed: 2026-11-16
  - url: https://docs.python.org/3/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Expressions
    locator: 6.10.1 Value comparisons (sequences)
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-tuples
    title: C# language reference, Tuple types
    locator: the introduction; Tuple equality
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/types/records
    title: C# fundamentals, Record types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Strict_equality
    title: MDN Web Docs, Strict equality (===)
    locator: Description
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.1p8
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.2.5p20; 6.5.9p2 (the operands of ==); 6.7.2.1p8 (no named members), p15 (order and padding)
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dataclasses.html
    title: The Python Standard Library, dataclasses
    locator: the introduction
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/tuple.general
    title: Working Draft, Standard for Programming Language C++ (N4950), [tuple.general]
    locator: 22.4.1 In general
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/data-classes.html#standard-data-classes
    title: Kotlin documentation, Data classes
    locator: Standard data classes
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/intro.object
    title: Working Draft, Standard for Programming Language C++ (N4950), [intro.object]
    locator: 6.7.2 Object model, paragraph 8 (nonzero size)
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Record_(computer_science)
    title: Record (computer science), Wikipedia
    locator: History
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/ALGOL_W
    title: ALGOL W, Wikipedia
    locator: the infobox; the introduction
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Tuple
    title: Tuple, Wikipedia
    locator: Uses in computer science (the introduction)
    accessed: 2026-11-16
---

## Idea

A product type holds several values at the same time. A point has an x coordinate and a y coordinate. A playing card has a rank and a suit. Each value of the type has exactly one value for each part.

The name comes from sets. "The set of all possible values of a product type is the Cartesian product of the sets of its component types."[^1] So the number of values of a product is the product of the numbers of values of its parts. Three ranks and two suits give six cards.

```python run id=idea
from dataclasses import dataclass
from itertools import product


@dataclass(frozen=True)
class Card:
    rank: str
    suit: str


ranks = ["A", "K", "Q"]
suits = ["clubs", "hearts"]
cards = [Card(r, s) for r, s in product(ranks, suits)]

print(len(cards), "cards from", len(ranks), "ranks and", len(suits), "suits")
print(cards[0])
print("equal to Card('A', 'clubs'):", "yes" if cards[0] == Card("A", "clubs") else "no")
```

```output for=idea
6 cards from 3 ranks and 2 suits
Card(rank='A', suit='clubs')
equal to Card('A', 'clubs'): yes
```

Products come in two forms. A tuple names its parts by position: the first part, the second part. A record or a struct names its parts by field name. In many languages, "product types take the form of a record type, for which the components of a tuple can be accessed by label".[^2]

## How it works

**Build and take apart.** A product has one way to build a value: give a value for each part. It has one way to read a part: a projection, which selects one component by position or by name. A tuple `t` in Rust has the parts `t.0` and `t.1`. A struct `p` has the parts `p.x` and `p.y`. Rust names the difference: "Unlike with tuples, in a struct you'll name each piece of data so it's clear what the values mean."[^3] Many languages can also take apart a product in one step, as in `let (x, y) = t;` or `x, y = t`.

**Counting.** A product of a type with `a` values and a type with `b` values has `a × b` values. A pair `(a, b)` "has an inhabitant for each combination of a s and b s".[^4] So a product of two booleans has 4 values, and a struct of two 32-bit integers has 2^64 values.

**The unit type.** A product with no parts has exactly one value. This is the unit type, "the product of no types".[^5] In Rust, "The tuple without any values has a special name, unit", and its value and its type are both written `()`.[^6] A function that returns nothing useful returns the unit. The unit acts as the number 1 for products: a product of `A` and the unit has as many values as `A`.

**Order.** The parts of a product have a fixed order: "the structure of a product type is determined by the fixed order of the operands in the product".[^2] So `(int, string)` and `(string, int)` are different types, even though they hold the same information. A record also has an order of fields. It shows in positional constructors, in printing and in memory.

**Equality.** Two values of a product are equal when each pair of corresponding parts is equal. The languages state this rule in their own words:

- Go: "Two struct values are equal if their corresponding non-blank field values are equal."[^7]
- Java: the `equals` method of a record returns true "if and only if the argument is an instance of R, and the current instance is equal to the argument instance at every record component of R".[^8]
- Python: "Sequences compare lexicographically using comparison of corresponding elements."[^9]
- C#: "Tuple types support equality operators == and != ."[^10] The `record` modifier makes the compiler "generate value equality".[^11]

Some languages do not compare products by value. In JavaScript, `===` on two objects returns true "only if they refer to the same object".[^12] In C, `==` does not accept two structures at all. Its operands must be arithmetic values or pointers.[^13] The Pitfalls show both cases.

**Memory.** A product stores its parts next to each other. In C, the members of a structure "have addresses that increase in the order in which they are declared", and "There may be unnamed padding within a structure object, but not at its beginning".[^13] So the size of a struct is at least the sum of the sizes of its members.

## In languages

The example files build a product by position and by name, read its parts, compare two values and show the unit, where the language has one.

| Language | Verdict | By position | By name | Unit |
|---|---|---|---|---|
| Python | native | `tuple` | a `dataclass`[^14] or `NamedTuple` | `()` |
| JavaScript | idiom | an array | an object | none |
| TypeScript | native | a tuple type `[number, string]` | an object type | `[]` as a type |
| Java | native | none | a `record` | a `record` with no components |
| C | native | none | a `struct` | none in ISO C[^13] |
| C++ | stdlib | `std::pair`, `std::tuple`[^15] | a `struct` | `std::tuple<>` |
| C# | native | a tuple `(int, int)`[^10] | a `record` or a `struct` | a `record` with no members |
| Go | native | none | a `struct` | `struct{}`, size 0[^7] |
| Rust | native | a tuple `(i32, i32)`[^6] | a `struct`[^3] | `()`[^6] |
| Kotlin | native | `Pair`, `Triple`[^16] | a `data class`[^16] | `Unit` |
| PowerShell | idiom | an array | a `[pscustomobject]` or a class | none |
| Bash | n/a | no types | no types | no types |
| SQL | native | a row value `(1, 'a')` | a row of a table | none |
| Regex | n/a | a pattern has no types | | |
| HTML/CSS | n/a | markup has no types | | |

**C.** A structure "describes a sequentially allocated nonempty set of member objects".[^13] A structure with no named members has undefined behavior in ISO C, so C has no unit type.[^13] Some compilers accept an empty struct as an extension.

**C++.** A `struct` with no data members is allowed, but a complete object of it has a nonzero size, and unless it is a bit-field, "an object with nonzero size shall occupy one or more bytes of storage".[^17] `std::tuple` gives "heterogeneous, fixed-size collections of values".[^15]

**Go.** Go has no tuple type. A function can return several results, but the results are not one value. The empty struct `struct{}` is the unit. "A struct or array type has size zero if it contains no fields (or elements, respectively) that have a size greater than zero."[^7]

**Python.** A `tuple` is a product by position. The `dataclasses` module adds generated methods such as `__init__()` and `__repr__()` to a class with typed fields, and so makes a product by name.[^14] A frozen dataclass also compares its fields for equality.

## Variations and related ideas

- **Records.** A product with named fields. See [[record-type]].
- **Tuples.** A product with numbered parts. See [[tuple]].
- **Sums.** The other operation of algebraic data types: one value of one of several cases. See [[sum-types]] and [[algebraic-data-type]].
- **Arrays.** An array of length n with elements of type T is a product of n copies of T. Its parts are numbered at run time, not by the type.
- **Dependent pairs.** In a dependent pair, the type of the second part can depend on the value of the first part.[^2] See [[dependent-types]].
- **Objects.** A class with fields is a product, with methods and often a hidden identity. Two objects with equal fields can still be different objects.

## Pitfalls

### Comparing two objects by reference

In JavaScript, two objects with the same fields are different values for `===`. The comparison checks if both operands are the same object.[^12]

```javascript wrong id=js-identity
const p = { x: 3, y: 4 };
const q = { x: 3, y: 4 };
console.log(p === q ? "equal" : "not equal");
```

```output for=js-identity
not equal
```

The fix is a function that compares each field, as the JavaScript example file does. Java has the same trap: `==` on two records compares references, and only `equals` compares the components.[^8]

### Two parts of the same type in the wrong order

A tuple names its parts by position only. When two parts have the same type, the compiler cannot see that they are swapped.

```python wrong id=swapped
def area_and_ratio(size: tuple[int, int]) -> str:
    width, height = size
    return f"area {width * height}, width/height {width / height:.2f}"


screen = (1080, 1920)  # the author meant (height, width)
print(area_and_ratio(screen))
```

```output for=swapped
area 2073600, width/height 0.56
```

The area is correct by chance, because multiplication does not depend on the order. The ratio of a 1920 by 1080 screen is 1.78. The fix is a record with named fields, such as `Size(width=1920, height=1080)`, so that each value states which part it is.

### Equality of floating-point parts

A product inherits the equality of its parts. In Java, the `equals` method of a record compares a `double` component with `Double.compare`, not with `==`.[^8] So a record that holds `NaN` is equal to another record that holds `NaN`, but `NaN == NaN` is false. Code that compares the fields by hand and code that calls `equals` can give different answers.

## Cost

A product stores each part once, plus any padding between parts.[^13] Construction writes each part. A projection is a read at a fixed offset, which the compiler knows. Equality compares the parts one by one and stops at the first pair that differs.[^7] A product that is a value type, such as a C struct, a Go struct, a Rust tuple or a C# tuple, is copied part by part on each assignment. C# tuple types "are value types; tuple elements are public fields".[^10] A product that is an object on the heap, such as a Java record or a Python dataclass, is copied as a reference.

## History

Records came from data processing. "COBOL was the first widespread programming language to support record types."[^18] Early numeric languages such as FORTRAN and ALGOL 60 did not have them.[^18] ALGOL W, which first appeared in 1966 and was designed by Niklaus Wirth and Tony Hoare, added "reference to record data types" to ALGOL 60.[^19] Pascal took its records from ALGOL W, and C provides records as structs.[^18][^19] Tuples as product types are a part of the typed functional languages: "Most typed functional programming languages implement tuples directly as product types".[^20]

## References
