---
tag: 8JECRW
kind: concept
slug: literal-types
name: Literal and singleton types
aliases: [literal type, singleton type, literal types, singleton types, Literal, typing.Literal, string literal type, numeric literal type, boolean literal type, union of literals, as const, integral_constant]
area: types/forms-of-types
level: advanced
summary: >-
  A literal type, or singleton type, has one value, such as "left" or 42. A union of literal types lists the values that a variable may hold, and the checker rejects any other value.
applies_to: core-languages
viz: optional
formal: none
see_also: [union-type, enumeration, unit-type, sum-types, exhaustive-matching, make-illegal-states-unrepresentable, dependent-types, subtyping]
sources:
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html
  title: Everyday Types, The TypeScript Handbook
  locator: Literal Types; Literal Inference
- url: https://docs.scala-lang.org/sips/42.type.html
  title: SIP-23, Literal-based singleton types, Scala Documentation
  locator: History; Introduction; Implementation status; Proposal summary
- url: https://peps.python.org/pep-0586/
  title: PEP 586, Literal Types
  locator: Abstract; Shortening unions of literals; Interactions with enums and exhaustiveness checks; Related work
- url: https://www.typescriptlang.org/docs/handbook/enums.html
  title: Enums, The TypeScript Handbook
  locator: Union enums and enum member types
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 8
  locator: 8.9 Enum Classes
- url: https://doc.rust-lang.org/reference/items/generics.html
  title: Generic parameters, The Rust Reference
  locator: Const generics
- url: https://en.cppreference.com/w/cpp/types/integral_constant.html
  title: std::integral_constant, C++ reference, cppreference.com
  locator: Description
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: typing.Literal
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/enum
  title: Enumeration types, C# language reference
  locator: Introduction; Conversions
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Constants; Iota
---

## Idea

Many parameters take only a few values: an alignment is "left", "right" or "center", and a comparison returns -1, 0 or 1. A type such as `string` or `int` allows every other value too, so a misspelled "centre" passes the type check and fails later. A literal type has one value, the literal itself. A union of literal types lists the exact values that are allowed, so a checker can reject "centre" before the program runs. Python spells this `Literal["left", "right", "center"]`. A type checker reads the annotation, and the program can also read the values when it runs.

```python run id=idea
from typing import Literal, get_args

Align = Literal["left", "right", "center"]


def pad(text: str, width: int, align: Align) -> str:
    match align:
        case "left":
            return text.ljust(width, ".")
        case "right":
            return text.rjust(width, ".")
        case "center":
            return text.center(width, ".")


print(pad("hi", 6, "left"), pad("hi", 6, "right"), pad("hi", 6, "center"))
print(get_args(Align))

# pad("hi", 6, "centre")  # a type checker rejects this argument
user_input = "centre"
print(user_input in get_args(Align))
print(Literal["a", "b"] == Literal["b", "a"], Literal[1, Literal[2]] == Literal[1, 2])
```

```output for=idea
hi.... ....hi ..hi..
('left', 'right', 'center')
False
True True
```

`Align` allows three strings and no others. A type checker such as mypy rejects the call with "centre", and it knows that the `match` covers every case, so the function always returns a string. Python does not check the annotation when the program runs, so a value that comes from outside, such as user input, must be tested against `get_args(Align)`. The last line shows that a `Literal` with several values acts as a set: the order does not matter, and nested literals flatten.

## How it works

**A value in a type position.** In TypeScript, "In addition to the general types string and number, we can refer to specific strings and numbers in type positions"[^1]. In Scala, a literal can "appear in type position, designating the corresponding singleton type"[^2], so `val one: 1 = 1` declares a value whose type is `1`[^2]. In Python, "Literal types indicate that some expression has literally a specific value"[^3]. A literal type is a [subtype](/subtyping/) of the type of its value: the type `"left"` is a subtype of `string`.

**Singleton types.** Scala describes "Singleton types, types which have a unique inhabitant"[^2], and says that "by pairing a type with its unique inhabitant, singleton types bridge the gap between types and values"[^2]. A literal type is a singleton type whose value is a literal. In Scala, the type of an `object` definition is also a singleton type[^2]. The [unit type](/unit-type/) is a singleton type too, but it has one value in total, while a language with literal types has a singleton type for each literal.

**Unions of literals.** One literal type alone is of little use, "But by combining literals into unions, you can express a much more useful concept - for example, functions that only accept a certain set of known values"[^1]. In Python, a `Literal` with several values "is equivalent to Union[Literal[v1], Literal[v2], Literal[v3]]"[^3] for `Literal[v1, v2, v3]`. In TypeScript, "The type boolean itself is actually just an alias for the union true | false"[^1]. See [union type](/union-type/).

**Inference and widening.** A checker must decide when to give a literal its own type and when to widen it to the general type. In TypeScript, a `const` string "can only represent 1 possible string"[^1], so it gets a literal type, but a property of an object can change, so "obj.counter must have the type number, not 0, because types are used to determine both reading and writing behavior"[^1]. "The as const suffix acts like const but for the type system, ensuring that all properties are assigned the literal type instead of a more general version like string"[^1].

**Enums as unions of literals.** An enumeration lists a fixed set of named values, so a checker can treat it as a union of one type for each member. In TypeScript, when all members have literal values, "enum members also become types as well!"[^4], and "enum types themselves effectively become a union of each enum member"[^4]. PEP 586 makes the same move for Python: a checker can "treat them as being approximately equivalent to the union of their values"[^3], so that the code for unions gives [exhaustive checks](/exhaustive-matching/) on enums too. Java has no literal types, but an enum class is "a restricted kind of class that defines a small set of named class instances"[^5]. See [enumeration](/enumeration/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `typing.Literal`, checked by a static type checker; `enum.Enum` for named sets |
| JavaScript | absent | no static types; a frozen object of constants and a test when the program runs |
| TypeScript | native | string, number, boolean and enum literal types; unions of them; `as const` |
| Java | idiom | an `enum` class lists a fixed set of instances; `switch` can check that it covers them |
| Kotlin | idiom | `enum class` for a fixed set; an `object` declaration has exactly one instance |
| C# | idiom | `enum` names constants of an integer type, but any value of that type converts to it |
| Go | idiom | a named integer type with constants from `iota`; any value of the type is allowed |
| Rust | idiom | an `enum` with unit variants; const generics put a constant value in a type |
| C | idiom | `enum` names `int` constants; the compiler does not limit a variable to them |
| C++ | stdlib | `std::integral_constant` is a type for one value; `enum class` for named sets |
| Bash | n/a | Bash has no types |
| PowerShell | idiom | .NET enums, as in C# |
| SQL | idiom | a `CHECK` constraint with `IN` limits a column to listed values |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS keywords are fixed values, but CSS has no type system for them |

TypeScript and Scala have literal types in the language, and Python adds them through `typing` for static checkers. Java, C#, Kotlin, Go, Rust and C get a closed set of values from an enumeration, which is a separate type, not a union of literals. C++ can make a type for one constant with a template.

The example files show these differences. Python shows `Literal` with enum members and with `Final`. Java shows an enum and a `switch` that must cover every constant. C# shows that a cast can put a value outside the enum into an enum variable. Go shows that a constant outside the set still has the named type. Rust shows an enum and a const generic parameter that makes `Mod<5>` and `Mod<7>` different types. C shows that an `enum` variable accepts any `int`. C++ shows `std::integral_constant` and `std::true_type`.

## Variations and related ideas

- **Discriminated unions.** A literal-typed field, such as `kind: "circle"`, tells a checker which case of a union a value belongs to. In the TypeScript enum example, an interface with `kind: ShapeKind.Circle` rejects an object whose `kind` is `ShapeKind.Square`[^4]. See [sum types](/sum-types/).
- **Values as type arguments.** C++ templates and Rust const generics take a constant as an argument to a type. "Const generic parameters allow items to be generic over constant values"[^6], so `[u8; 4]` and `[u8; 8]` are different types. "std::integral_constant wraps a static constant of specified type. It is the base class for the C++ type traits"[^7]. Full [dependent types](/dependent-types/) go further and let a type depend on a value computed at run time.
- **Literal strings.** Python has `LiteralString`, a "Special type that includes only literal strings"[^8]. A string built from other literal strings is also a `LiteralString`, but a plain `str` is not, so a query function that takes a `LiteralString` rejects text from user input, which "could be vulnerable to an SQL injection attack"[^8].
- **Singleton objects.** A Scala `object` or a Kotlin `object` declaration has a type with one instance. This is the singleton pattern built into the language, and in Scala the type of the object is a singleton type[^2].

## Pitfalls

- **Widening loses the literal.** A string in a mutable object widens to `string`, so passing `req.method` where `"GET" | "POST"` is expected fails in TypeScript[^1]. Use `as const`, or annotate the object.
- **An enum is not a closed set in C#, C and Go.** In C#, "explicit conversions exist between the enumeration type and its underlying integral type"[^9], so `(Color)42` is a valid `Color` with no name. The C# documentation advises: "Use Enum.IsDefined to validate enum values when converting from numeric types"[^9]. In Go, `iota` "represents successive untyped integer constants"[^10], and any other integer of the named type is allowed too. The C#, C and Go examples show this.
- **No check when the program runs.** Python and TypeScript do not check literal types when the program runs. A value from a file, a form or the network must be tested against the allowed values, as the Python idea block does with `get_args`.

## Cost

Literal types cost nothing when the program runs in TypeScript and Python, because the checker removes them. An enumeration in Java is a class with one object for each constant, and in C#, C and Go it is an integer[^5][^9][^10]. In C++ and Rust, the constant in a type such as `std::integral_constant<int, 42>` or `[u8; 4]` is known when the program compiles[^7][^6].

## History

Scala's type system had singleton types for paths from its earliest published work, and SIP-23 proposed literal types in 2014[^2]. Literal types were implemented in Typelevel Scala and in Dotty[^2]. TypeScript has string, number and boolean literal types[^1]. PEP 586, created in March 2019 for Python 3.8, added `typing.Literal`, and its authors wrote that its design ended up "converging into something similar to how literal types are handled in TypeScript"[^3].

## References
