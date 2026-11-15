---
tag: 8RDPSX
kind: concept
slug: tuple
name: Tuple
aliases: [tuple, tuple type, n-tuple, pair, triple, ordered pair, value tuple]
area: types/algebraic-data-types
level: intro
summary: >-
  A tuple groups a fixed number of values by position. Each position can have its own type. Programs read the parts by index or unpack them into variables.
applies_to: core-languages
viz: optional
formal: none
see_also: [product-type, record-type, algebraic-data-type, type, pattern-matching]
same_as:
  wikidata: Q600590
  wikipedia: Tuple
sources:
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html#the-tuple-type
    title: The Rust Programming Language, 3.2 Data Types
    locator: The Tuple Type
    accessed: 2026-11-16
  - url: https://docs.python.org/3/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, 5. Data Structures
    locator: 5.3 Tuples and Sequences; 5.5 Dictionaries
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
    title: MDN Web Docs, Destructuring
    locator: the introduction
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/structured_binding.html
    title: cppreference.com, Structured binding declaration
    locator: the introduction
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html#tuple-types
    title: TypeScript Handbook, Object Types
    locator: Tuple Types
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-tuples
    title: C# language reference, Tuple types
    locator: the introduction; Tuple field names; Tuple equality
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/collections.html#collections.namedtuple
    title: The Python Standard Library, collections, namedtuple()
    locator: namedtuple() Factory Function for Tuples with Named Fields
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/typing.html#typing.NamedTuple
    title: The Python Standard Library, typing, NamedTuple
    locator: class typing.NamedTuple
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/tuple.general
    title: Working Draft, Standard for Programming Language C++ (N4950), [tuple.general]
    locator: 22.4.1 In general
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#multiple-returns
    title: Effective Go
    locator: Multiple return values
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/data-classes.html#standard-data-classes
    title: Kotlin documentation, Data classes
    locator: Standard data classes
    accessed: 2026-11-16
  - url: https://openjdk.org/jeps/395
    title: "JEP 395: Records"
    locator: Records and sealed classes; Summary
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Tuple
    title: Tuple, Wikipedia
    locator: the introduction; Etymology
    accessed: 2026-11-16
---

## Idea

A tuple is a group of values by position. The pair `(2, 9)` has two parts: the first is 2, the second is 9. The Rust book says that a tuple "is a general way of grouping together a number of values with a variety of types into one compound type".[^1] The number of parts is part of the type: "Tuples have a fixed length: Once declared, they cannot grow or shrink in size."[^1]

```python run id=idea
def min_max(values: list[int]) -> tuple[int, int]:
    return min(values), max(values)


pair = min_max([7, 2, 9, 4])
print(pair)

lo, hi = pair
print("lo:", lo, "hi:", hi)
print("second part:", pair[1])
```

```output for=idea
(2, 9)
lo: 2 hi: 9
second part: 9
```

A tuple is the simplest product type (see [[product-type]]). It has no names, only positions. That makes it short to write, and a good fit for a small result such as a minimum and a maximum. For data with many parts, or data that lives long, a record with named fields is clearer (see [[record-type]]).

## How it works

**Build.** Most languages write a tuple as values in parentheses with commas between them, as in `(2, 9)`. Python needs only the commas: "A tuple consists of a number of values separated by commas".[^2] TypeScript and JavaScript use an array literal, as in `[2, 9]`.

**Read by position.** Each part has an index, which starts at 0 in most languages. The syntax differs: `t[1]` in Python, `t.1` in Rust, `t.Item2` in C#, `std::get<1>(t)` in C++.

**Unpack.** A program can bind each part to its own variable in one statement. Python calls this "sequence unpacking", and it "requires that there are as many variables on the left side of the equals sign as there are elements in the sequence".[^2] JavaScript has destructuring, a syntax that "makes it possible to unpack values from arrays, or properties from objects, into distinct variables".[^3] C++ has structured bindings, which bind names "to subobjects or elements of the initializer".[^4] In Rust and the ML family, a tuple pattern in a `let` or a `match` does the same.

**Type.** The type of a tuple lists the type of each position, as in `(i32, f64)` in Rust, `tuple[int, str]` in Python or `[string, number]` in TypeScript. TypeScript describes a tuple type as an array type "that knows exactly how many elements it contains, and exactly which types it contains at specific positions".[^5] Two tuple types with the same parts in a different order are different types.

**Equality.** Two tuples are equal when they have the same number of parts and the parts at each position are equal. C# says that `==` and `!=` "compare members of the left-hand operand with the corresponding members of the right-hand operand following the order of tuple elements".[^6]

**The empty tuple.** A tuple with no parts has only one value. Rust calls it unit: "This value and its corresponding type are both written () and represent an empty value or an empty return type."[^1]

**Named tuples.** Some languages let a tuple also have names for its positions. Python's `namedtuple` makes "tuple-like objects that have fields accessible by attribute lookup as well as being indexable and iterable".[^7] C# lets a program name the elements, as in `(Sum: 4.5, Count: 3)`.[^6] A named tuple is close to a record, but it can still be read by position.

## In languages

The example files return a minimum and a maximum as a pair, unpack the pair and compare two pairs.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `tuple`, immutable; `namedtuple` and `typing.NamedTuple` for names[^2][^7][^8] |
| JavaScript | idiom | an array of fixed length, unpacked with destructuring[^3] |
| TypeScript | native | a tuple type such as `[number, number]`[^5] |
| Java | idiom | a small `record`, or `Map.entry` for a pair |
| C | idiom | a `struct` with one member for each position |
| C++ | native | `std::pair` and `std::tuple`, with structured bindings[^9][^4] |
| C# | native | value tuples such as `(int, int)`[^6] |
| Go | idiom | multiple return values, or a small `struct`[^10] |
| Rust | native | tuple types such as `(i32, i32)`[^1] |
| Kotlin | native | `Pair` and `Triple`[^11] |
| PowerShell | idiom | an array, or `[System.Tuple]` from .NET |
| Bash | idiom | separate variables, or a function that prints several words |
| SQL | native | a row value such as `(1, 'a')` |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types |

**Python.** Tuples "are immutable, and usually contain a heterogeneous sequence of elements that are accessed via unpacking".[^2] `typing.NamedTuple` is the "Typed version of collections.namedtuple()".[^8]

**C++.** In the standard, "tuples are heterogeneous, fixed-size collections of values".[^9] A `std::pair` is a tuple with two elements.

**C#.** A tuple such as `(double, int)` is a value type, and "tuple elements are public fields. That design makes tuples mutable value types."[^6]

**Java.** Java has no tuple type. JEP 395 says that "Records can be thought of as nominal tuples".[^12] So a Java program declares a small record, such as `record MinMax(int min, int max)`, in place of a pair.

**Go.** Go has no tuple type, but "functions and methods can return multiple values".[^10] The results must be assigned to separate variables at the call. A program that must store the parts together uses a struct.

**Kotlin.** "The standard library provides the Pair and Triple classes." The same page says that named data classes are usually the better design.[^11]

## Variations and related ideas

- **Records.** A product whose parts have names. Wikipedia describes records as an alternative to tuples, "featuring unordered elements accessed by label".[^13] See [[record-type]].
- **Unit.** The empty tuple, the type with one value. See [[product-type]].
- **Variadic tuples.** C++ `std::tuple` and TypeScript tuple types can have any number of elements, and generic code can work on the whole list of types.
- **Relational tuples.** Relational databases can call a row of a table a tuple.[^13]
- **Arrays.** An array has many elements of one type, and its length is often not part of its type. A tuple has a fixed number of parts, each with its own type.

## Pitfalls

### A one-part tuple needs a comma

In Python, parentheses alone do not make a tuple. The tutorial says that "a tuple with one item is constructed by following a value with a comma (it is not sufficient to enclose a single value in parentheses)".[^2] Here `names` is a string, so the loop runs over its characters.

```python wrong id=single
names = ("Ada")
for n in names:
    print(n)
```

```output for=single
A
d
a
```

The fix is a trailing comma: `names = ("Ada",)`.

### Positions do not carry meaning

A tuple says nothing about what each position means. A pair `(lat, lon)` and a pair `(lon, lat)` have the same type, so a call that swaps them compiles and gives wrong results. In C#, element names do not help with equality: "the == and != operations don't take tuple field names into account".[^6] So `(A: 5, B: 10) == (B: 5, A: 10)` is true. When the meaning of each part matters, use a record with named fields.

### A Python tuple can hold mutable parts

A tuple is immutable, but its parts can be mutable. The tutorial shows a tuple of two lists: tuples "can contain mutable objects".[^2] A change inside one of the lists changes what the tuple shows. Such a tuple also cannot be a dictionary key: "if a tuple contains any mutable object either directly or indirectly, it cannot be used as a key".[^2]

## Cost

A tuple stores each part once. In C++, C#, Go, Rust and C, a tuple or a small struct is a value with no header, so it can live in registers or on the stack. A C# value tuple is copied on assignment. In Python and JavaScript, a tuple or an array is an object on the heap that holds references to its parts. Unpacking a tuple costs one read for each part.

## History

The word comes from mathematics. Wikipedia says that it "originated as an abstraction of the sequence: single, couple/double, triple, quadruple, quintuple, sextuple".[^13] In programming, "Most typed functional programming languages implement tuples directly as product types".[^13] Many later languages added tuples to the standard library or to the core language, such as `std::tuple` in C++ and value tuples in C#.[^9][^6]

## References
