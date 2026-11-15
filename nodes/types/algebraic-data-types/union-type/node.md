---
tag: N42S89
kind: concept
slug: union-type
name: Union type
aliases: [union, untagged union, C union, type union, union of types]
area: types/algebraic-data-types
level: core
summary: >-
  A union type has values that can be of any one of several types. A C union shares one block of memory between its members, and a TypeScript or Python union lists the allowed types.
applies_to: core-languages
viz: optional
formal: none
see_also: [sum-types, algebraic-data-type, enumeration, type, subtyping, pattern-matching]
same_as:
  wikidata: Q893290
  wikipedia: Union_type
sources:
  - url: https://en.wikipedia.org/wiki/Union_type
    title: Union type, Wikipedia
    locator: the introduction; Untagged unions; Unions in various programming languages
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.1
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.2.5p20; 6.7.2.1p16; 6.5.2.3 footnote 95
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#union-types
    title: TypeScript Handbook, Everyday Types
    locator: Union Types; Working with Union Types
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0604/
    title: "PEP 604: Allow writing union types as X | Y"
    locator: the header (Python-Version 3.10); Abstract
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Tagged_union
    title: Tagged union, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html#types-union
    title: The Python Standard Library, Built-in Types
    locator: Union Type; Boolean Type
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/union.html
    title: cppreference.com, Union declaration
    locator: Explanation (the active member)
    accessed: 2026-11-16
  - url: https://timsong-cpp.github.io/cppwp/n4950/variant.variant.general
    title: Working Draft, Standard for Programming Language C++ (N4950), [variant.variant.general]
    locator: 22.6.3.1 General
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/union
    title: C# language reference, Union types
    locator: the introduction
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-15
    title: What's new in C# 15
    locator: the introduction; Union types
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Interface_types
    title: The Go Programming Language Specification
    locator: Interface types, General interfaces
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/items/unions.html
    title: The Rust Reference, Unions
    locator: Reading and writing union fields
    accessed: 2026-11-16
---

## Idea

Sometimes a value can have one of several types. An ID can be a number or a string. A setting can be a Boolean or a list of names. A union type says exactly that. Wikipedia says that "a union type specifies the permitted types that may be stored in its instances, e.g., float and int".[^1]

```python run id=idea
def describe(value: int | str) -> str:
    if isinstance(value, int):
        return f"number {value}, doubled {value * 2}"
    return f"text {value}, upper {value.upper()}"


for v in [21, "ok"]:
    print(describe(v))
```

```output for=idea
number 21, doubled 42
text ok, upper OK
```

Before the code uses the value, it must find out which type the value has. Here `isinstance` does the check. After the check, the code can use the operations of that one type.

The name comes from sets: "If a type is considered as the set of all values that that type can take on, a union type is simply the mathematical union of its constituting types".[^1]

## How it works

The word union names two different things in programming.

**A C union: shared memory.** In C, "A union type describes an overlapping nonempty set of member objects".[^2] All members start at the same address. "The size of a union is sufficient to contain the largest of its members. The value of at most one of the members can be stored in a union object at any time."[^2] The union itself does not record which member holds the value. The program must keep that information somewhere else. This is an untagged union.

**A union of types: a set of allowed types.** In TypeScript, "A union type is a type formed from two or more other types, representing values that may be any one of those types."[^3] Python writes the same idea as `int | str`.[^4] Here each value still knows its own type at run time, so the program can test the type with `typeof` or `isinstance`. The type checker uses the test to narrow the union to one member type. Before the test, "TypeScript will only allow an operation if it is valid for every member of the union".[^3]

**Tagged unions.** A tagged union adds a tag that says which case is in use. Wikipedia describes a tagged union as "a data structure used to hold a value that could take on several different, but fixed, types", in which "a tag field explicitly indicates which type is in use".[^5] A C program builds one from a struct with an enum tag and a union. In TypeScript, a common field with literal types does the job: "When every type in a union contains a common property with literal types, TypeScript considers that to be a discriminated union".[^6] A tagged union is a sum type; see [[sum-types]].

| | Untagged union (C) | Union of types (TypeScript, Python) | Tagged union (sum type) |
|---|---|---|---|
| Which case is in use | the program must track it | the run time type of the value | a tag stored with the value |
| Check before use | none; a wrong read is a bug | `typeof`, `isinstance`, a type pattern | a `match` or a `switch` on the tag |
| Storage | the largest member | a reference to the value | the largest case plus the tag |

## In languages

The example files take a value that is an integer or a string, find out which, and use it.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `int \| str`, or `typing.Union[int, str]`[^7][^4] |
| JavaScript | idiom | any variable can hold any type; `typeof` tells them apart |
| TypeScript | native | `number \| string`; discriminated unions with a common field[^3][^6] |
| Java | idiom | `Object` with type patterns, or a sealed interface |
| C | native | `union`, untagged[^2] |
| C++ | native | `union`, untagged; `std::variant` as a tagged union[^8][^9] |
| C# | native | `union` types from C# 15[^10][^11] |
| Go | idiom | an interface type and a type switch; union terms only in type constraints[^12] |
| Rust | native | `union`, untagged and `unsafe` to read; `enum` as a tagged union[^13] |
| Kotlin | idiom | a sealed interface, or `Any` with `when` and `is` checks |
| PowerShell | idiom | any variable can hold any type; `-is` tells them apart |
| Bash | n/a | every value is a string |
| SQL | n/a | each column has one type |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types |

**C++.** A raw union has an active member, the one most recently written. cppreference warns: "It is undefined behavior to read from the member of the union that wasn't most recently written."[^8] `std::variant` is the safe form. "Any instance of variant at any given time either holds a value of one of its alternative types or holds no value."[^9]

**Rust.** A Rust `union` has no active member: "every union access just interprets the storage as the type of the field used for the access".[^13] So every read of a union field must be in an `unsafe` block.[^13] Most Rust code uses an `enum`, which is a tagged union that the compiler checks.

**C#.** C# 15 added union types: "A union type represents a value that can be one of several case types."[^10] C# 15 is the default language version for projects that target .NET 11.[^11] The example file uses `object` and type patterns, which work in older versions too.

**Go.** An interface can list type terms such as `int | string`, but "Interfaces that are not basic may only be used as type constraints".[^12] So a variable cannot have such a type. A program uses `any` or another interface, and a type switch.

## Variations and related ideas

- **Sum types.** A tagged union with a fixed set of cases that the compiler checks. See [[sum-types]] and [[algebraic-data-type]].
- **Enumerations.** A tagged union whose cases carry no data. See [[enumeration]].
- **Optional values.** `T | None` in Python and `T | undefined` in TypeScript are unions of a type with a type that has one value. See [[option-type]].
- **Subtyping.** Each member of a union is a subtype of the union, so a value of a member type can be used where the union is expected. See [[subtyping]].

## Pitfalls

### Reading the wrong member of an untagged union

A C union does not know which member holds the value. If the program reads a different member from the one it wrote, C reinterprets the bytes: "the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type".[^2] The result is a different value, not an error.

```c wrong id=wrong-member
#include <stdio.h>

union number {
    int i;
    float f;
};

int main(void) {
    union number n;
    n.i = 42;
    printf("as int: %d, as float: %g\n", n.i, n.f);
    return 0;
}
```

```output for=wrong-member
as int: 42, as float: 5.88545e-44
```

The fix is a tag: put the union in a struct with an enum field that records which member holds the value, and check it before each read. In C++, the same read is undefined behavior, so use `std::variant`.[^8]

### Overlapping member types

A type test checks whether the value is an instance of a type, and a value can belong to two member types at once. In Python, "bool is a subclass of int".[^7] So in a union `int | bool`, the test for `int` also matches `True`.

```python wrong id=overlap
def describe(value: int | bool) -> str:
    if isinstance(value, int):
        return "number"
    return "flag"


print(describe(True))
```

```output for=overlap
number
```

The fix is to test the narrower type first: `if isinstance(value, bool)`. A tagged union does not have this problem, because the tag names the case.

### A union without a check

Code that uses a union value without a check fails at run time in a dynamic language and does not compile in TypeScript. In C, it compiles and reads the wrong bytes. Each use of a union value needs a check, so a union that many functions pass around spreads checks through the code. A sum type with a `match` in one place, or a method on each case, keeps the checks together.

## Cost

A C union takes the size of its largest member, plus any padding. It has no tag, so it needs no extra space, but the program must store the tag somewhere else. A tagged union adds the tag, often one byte or one word. A union of types in a dynamic language costs nothing extra in the value, because each value already carries its type; the check costs one type test. `std::variant` stores the largest alternative plus an index.

## History

Wikipedia lists ALGOL 68 among the early languages with unions: "ALGOL 68 has tagged unions, and uses a case clause to distinguish and extract the constituent type at runtime."[^1] COBOL can lay a second data item over the same memory.[^1] C has untagged unions. Wikipedia notes that "untagged unions are generally only provided in untyped languages or in a type-unsafe way (as in C)".[^1] Later typed languages added safe forms: `std::variant` in C++, union types in TypeScript, the `X | Y` syntax in Python 3.10, and union types in C# 15.[^9][^3][^4][^10]

## References
