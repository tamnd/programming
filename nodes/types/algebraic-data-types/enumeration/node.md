---
tag: DS22BP
kind: concept
slug: enumeration
name: Enumeration
aliases: [enum, enumerated type, enumeration type, enum type, enum class, enumerator]
area: types/algebraic-data-types
level: intro
summary: >-
  An enumeration is a type with a fixed set of named values, such as the colors of a traffic light. Some languages store the values as integers, and others as objects.
applies_to: core-languages
viz: optional
formal: none
see_also: [sum-types, algebraic-data-type, type, pattern-matching, boolean-type]
same_as:
  wikidata: Q760170
  wikipedia: Enumerated_type
sources:
  - url: https://en.wikipedia.org/wiki/Enumerated_type
    title: Enumerated type, Wikipedia
    locator: the introduction; Syntax by language, ALGOL 60 based languages
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.2
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.4.4.3 Enumeration constants; 6.7.2.2 Enumeration specifiers
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/enum
    title: C# language reference, Enumeration types
    locator: the introduction; Enumeration types as bit flags; Conversions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Iota
    title: The Go Programming Language Specification
    locator: Iota
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/tutorial/java/javaOO/enum.html
    title: The Java Tutorials, Enum Types
    locator: the whole page
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se21/html/jls-8.html#jls-8.9
    title: The Java Language Specification, Java SE 21 Edition, 8.9 Enum Classes
    locator: 8.9
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/enum.html
    title: The Python Standard Library, enum
    locator: the introduction; IntEnum; Flag
    accessed: 2026-11-16
  - url: https://kotlinlang.org/docs/enum-classes.html
    title: Kotlin documentation, Enum classes
    locator: the introduction; Working with enum constants
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/enums.html
    title: TypeScript Handbook, Enums
    locator: the introduction; Reverse mappings
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/enum.html
    title: cppreference.com, Enumeration declaration
    locator: Scoped enumerations
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch06-01-defining-an-enum.html
    title: The Rust Programming Language, 6.1 Defining an Enum
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/howto/enum.html
    title: Python HOWTOs, Enum HOWTO
    locator: Comparisons
    accessed: 2026-11-16
---

## Idea

Some values come from a short, fixed list. A traffic light is red, amber or green. A suit is clubs, diamonds, hearts or spades. An enumeration is a type whose values are exactly the items of such a list. Wikipedia defines it as "a data type consisting of a set of named values called elements, members, enumeral, or enumerators of the type".[^1]

```python run id=idea
from enum import Enum


class Light(Enum):
    RED = 0
    AMBER = 1
    GREEN = 2


def after(light: Light) -> Light:
    match light:
        case Light.RED:
            return Light.GREEN
        case Light.GREEN:
            return Light.AMBER
        case Light.AMBER:
            return Light.RED


for light in Light:
    print(light.name, light.value, "then", after(light).name)
```

```output for=idea
RED 0 then GREEN
AMBER 1 then RED
GREEN 2 then AMBER
```

An enumeration is better than a plain string or a plain number. The names show the meaning, and the type stops a program from using a value that is not in the list. Wikipedia notes that "An enumerated type can be seen as a degenerate tagged union of unit type".[^1] That is, it is a sum type whose variants carry no data (see [[sum-types]]).

## How it works

**Declaration.** A program lists the names once, as in `enum Light { RED, AMBER, GREEN }`. Each name becomes a constant of the new type.

**Integers or objects.** Languages store enumeration values in two ways:

- As integers. In C, "An identifier declared as an enumeration constant has type int", and each enumerated type "shall be compatible with char, a signed integer type, or an unsigned integer type".[^2] In C#, an enum "is a value type defined by a set of named constants of the underlying integral numeric type".[^3] C++ and TypeScript numeric enums also work this way. Go has no enum keyword: a program declares typed constants, and `iota` "represents successive untyped integer constants".[^4]
- As objects. In Java, "The enum declaration defines a class (called an enum type)", and its body "can include methods and other fields".[^5] The JLS says that an enum class "has no instances other than those defined by its enum constants".[^6] Python and Kotlin enumerations are classes too.[^7][^8]

**Iteration.** Many languages can list all the values of an enumeration in declaration order. In Java, the compiler adds "a static values method that returns an array containing all of the values of the enum in the order they are declared".[^5] A Python enumeration "can be iterated over to return its canonical (i.e. non-alias) members in definition order".[^7] Kotlin has an `entries` property. C, C++ and Go have no built-in way to list the values.

**Case analysis.** A `switch` or a `match` on an enumeration can have one branch for each value. Many compilers check that every value has a branch. For Kotlin: "When you use all enum constants in a when expression, you don't need an else branch."[^8] C and C++ compilers can warn about a missing case.

**Names and numbers.** Programs often convert an enumeration value to its name, to its number, and back. These conversions are where the two models differ most: an integer enum can hold a number that has no name, and an object enum cannot.

## In languages

The example files declare a `Light` enumeration with three values, print the name and the number of each value, and use a `switch` to find the next light.

| Language | Verdict | Form |
|---|---|---|
| Python | native | a subclass of `enum.Enum`; `IntEnum` and `Flag` for integer and flag enumerations[^7] |
| JavaScript | idiom | a frozen object of constants, or a set of string constants |
| TypeScript | native | `enum`, numeric or string; a union of string literal types is the common alternative[^9] |
| Java | native | `enum`, a class with a fixed set of instances[^5][^6] |
| C | native | `enum`, a set of `int` constants[^2] |
| C++ | native | `enum class` (scoped) and plain `enum` (unscoped)[^10] |
| C# | native | `enum`, with an integral underlying type[^3] |
| Go | idiom | a named integer type and a `const` block with `iota`[^4] |
| Rust | native | `enum`; a variant can also carry data[^11] |
| Kotlin | native | `enum class`[^8] |
| PowerShell | native | `enum` in a script or a module |
| Bash | idiom | a set of string or integer constants |
| SQL | idiom | a `CHECK` constraint or a lookup table; some systems have an `ENUM` type |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup has no types; an HTML `select` element lists fixed choices |

**C and C++.** A plain C `enum` mixes freely with `int`. C++ added scoped enumerations, written `enum class` or `enum struct`.[^10] For a scoped enumeration, "There are no implicit conversions from the values of a scoped enumerator to integral types, although static_cast may be used to obtain the numeric value of the enumerator."[^10]

**TypeScript.** "Enums are one of the few features TypeScript has which is not a type-level extension of JavaScript."[^9] A numeric enum compiles to an object that maps names to numbers and numbers back to names; the handbook calls this a reverse mapping.[^9] The handbook also says: "In modern TypeScript, you may not need an enum when an object with as const could suffice".[^9] The example file uses such an object. Many programs also use a union of string literal types, such as `"red" | "amber" | "green"`, in place of an enum.

**Rust.** The book says that "enums give you a way of saying a value is one of a possible set of values".[^11] A Rust enum whose variants carry no data is an enumeration. Its variants can also carry data, which makes it a full sum type.

## Variations and related ideas

- **Flag enumerations.** Each value is one bit, so a variable can hold a combination of values. In Python, `Flag` members "support the bitwise operators & (AND), | (OR), ^ (XOR), and ~ (INVERT)".[^7] C# uses the `[Flags]` attribute for an enum whose members are bit fields.[^3]
- **String enumerations.** The values are strings, not numbers. A TypeScript string enum gives "a meaningful and readable value when your code runs".[^9]
- **Enumerations with data.** When a variant also carries values, the type is a sum type. See [[sum-types]] and [[algebraic-data-type]].
- **Enumerations with methods.** Java and Kotlin enum constants can have fields, constructors and methods, so each constant can carry fixed data such as a color code.[^5][^8]
- **Booleans.** A Boolean type is an enumeration with two values. See [[boolean-type]].

## Pitfalls

### An integer enum can hold a value that has no name

In C, an enumeration variable has an integer type, so it can hold any number of that type. A `switch` that covers every name then misses the value.

```c wrong id=no-name
#include <stdio.h>

enum light { RED, AMBER, GREEN };

static const char *light_name(enum light l) {
    switch (l) {
    case RED:
        return "red";
    case AMBER:
        return "amber";
    case GREEN:
        return "green";
    }
    return "no name";
}

int main(void) {
    enum light l = 7; /* compiles: the type is an integer type */
    printf("%d %s\n", (int)l, light_name(l));
    return 0;
}
```

```output for=no-name
7 no name
```

C# has the same rule. Its reference shows a cast of 0 to an enum that has no member for 0, and it uses `Enum.IsDefined` to check whether a value has a name.[^3] Code that reads an enumeration value from a file, a network or a database must check it.

### A Python enum member is not equal to its value

A Python `Enum` member is an object, not a number. "Equality comparisons against non-enumeration values will always return False".[^12] Here the status comes from a database row as the number 1.

```python wrong id=compare
from enum import Enum


class Status(Enum):
    ACTIVE = 1
    BLOCKED = 2


row_status = 1  # read from a database
print("active" if row_status == Status.ACTIVE else "not active")
```

```output for=compare
not active
```

The fix is to convert the number first: `Status(row_status) == Status.ACTIVE`. `IntEnum` members are integers too, so they compare equal to numbers, but they lose the type check.[^7]

### Stored numbers depend on the order of the names

When the values come from the position of each name, as with C enums without explicit values or Go `iota`, a new name in the middle changes the numbers of the names after it.[^4] Numbers that were saved in a file or a database then mean different values. Give explicit values to any enumeration whose numbers leave the program, or store the names.

## Cost

An integer enumeration costs the same as its underlying integer: one word or less, compared with one machine instruction. An object enumeration, as in Java or Python, stores a reference to one of a fixed set of objects. Comparison is a reference comparison. A `switch` on an enumeration with dense values can compile to a jump table.

## History

Wikipedia notes that "While ALGOL 60 itself has no enumeration type, several languages descended from it do."[^1] In Pascal, a program can declare an enumerated type by "listing the values in a parenthesised list", as in `(clubs, diamonds, hearts, spades)`.[^1] C has `enum` with integer constants.[^2] C++11 added scoped enumerations.[^10] Python added the `enum` module in version 3.4.[^7]

## References
