---
tag: JW76G0
kind: concept
slug: boolean-type
name: Boolean type
aliases: [boolean, bool, Boolean, logical type, truth value, true and false, truthy, falsy, truthiness, LOGICAL]
area: data-and-values/primitive-types
level: core
summary: >-
  The Boolean type has two values, true and false. Comparisons produce them, conditions consume them, and the operators and, or and not combine them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John Backus, Peter Naur]
  note: ALGOL 60 had the type Boolean with the values true and false, and later languages such as Pascal, Java and C# took the type from it. The name comes from George Boole, who defined an algebra of logic in the 19th century.
see_also: [short-circuit-evaluation, type, type-conversion, operator, static-and-dynamic-typing]
same_as:
  wikidata: Q520777
  wikipedia: Boolean_data_type
sources:
- url: https://en.wikipedia.org/wiki/Boolean_data_type
  title: Boolean data type, Wikipedia
  locator: Introduction; ALGOL and the built-in BOOLEAN type; Fortran; C, C++, D, Objective-C, AWK
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.2.5 The boolean Type and boolean Values
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Boolean types
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Truth Value Testing; Boolean Type - bool
- url: https://developer.mozilla.org/en-US/docs/Glossary/Falsy
  title: Falsy, MDN Web Docs Glossary
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/language/implicit_conversion.html
  title: Implicit conversions, C++ reference, cppreference.com
  locator: Boolean conversions
- url: https://en.cppreference.com/w/c/language/arithmetic_types.html
  title: Arithmetic types, C reference, cppreference.com
  locator: Boolean type
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/bool
  title: bool, C# language reference
  locator: Introduction; Three-valued Boolean logic; Conversions
- url: https://doc.rust-lang.org/std/primitive.bool.html
  title: Primitive Type bool, The Rust Standard Library
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Boolean
  title: Boolean, MDN Web Docs
  locator: Boolean primitives and Boolean objects
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language ALGOL 60
  locator: 2.2.2 Logical values
- url: https://peps.python.org/pep-0285/
  title: PEP 285, Adding a bool type
  locator: Abstract
---

## Idea

A program must decide things: whether a file exists, whether a number is in range, whether a loop is done. The Boolean type holds the answer to such a question. It has exactly two values, true and false. A comparison such as `n > 0` produces one of them, an `if` or a `while` uses one to choose what runs next, and the operators `and`, `or` and `not` build larger conditions from smaller ones.

```python run id=idea
# A Boolean has two values, and comparisons produce them.
print(3 < 5, 3 == 5, type(3 < 5).__name__)

# and, or and not combine Booleans.
print("a     b     and   or    not a")
for a in (False, True):
    for b in (False, True):
        print(f"{a!s:5} {b!s:5} {a and b!s:5} {a or b!s:5} {not a}")

# A condition decides which branch runs.
n = 7
if n % 2 == 1 and n > 0:
    print("positive and odd")

# Python also tests other values for truth: zero and empty values are false.
print([bool(v) for v in (0, "", [], None, 1, "0", [0])])
```

```output for=idea
True False bool
a     b     and   or    not a
False False False False True
False True  False True  True
True  False False True  False
True  True  True  True  False
positive and odd
[False, False, False, False, True, True, True]
```

`3 < 5` is `True` and `3 == 5` is `False`, and both have the type `bool`. The table shows the rules of the three operators: `and` is true only when both operands are true, `or` is true when at least one is true, and `not` inverts its operand. The `if` statement runs its body because `n % 2 == 1 and n > 0` is true. The last line shows a rule of Python and some other languages: a value of another type also has a truth value, so `0`, `""`, `[]` and `None` count as false and `"0"` and `[0]` count as true.

## How it works

**Two values.** "In computer science, the Boolean is a data type that has one of two possible values representing the two truth values of logic: true and false"[^1]. The language references say the same. In Java, "The boolean type represents a logical quantity with two possible values, indicated by the literals true and false"[^2]. In Go, "A boolean type represents the set of Boolean truth values denoted by the predeclared constants true and false"[^3].

**Where Booleans come from and where they go.** Comparison operators such as `<`, `==` and `!=` produce Booleans, and so do functions such as `isinstance` and `contains`. Conditions consume them: `if`, `while`, `for`, the conditional operator `? :` and assertions. Java states the rule strictly: "Only boolean and Boolean expressions can be used in control flow statements and as the first operand of the conditional operator ? :"[^2].

**Operators.** `and` (`&&`), `or` (`||`) and `not` (`!`) are the main operators. In most languages, `&&` and `||` do not evaluate the right operand when the left one decides the result, so `p != null && p.ok` is safe. See [short-circuit evaluation](/short-circuit-evaluation/). Exclusive or is `^` in C, Java, C#, Go and Rust, or `!=` on two Booleans. The bitwise operators `&` and `|` also work on Booleans in Java, C#, Rust and Python, but they evaluate both operands.

**Truthiness.** Some languages let a condition test a value of any type. Python says that "Any object can be tested for truth value, for use in an if or while condition or as operand of the Boolean operations"[^4], and that the false values include `None`, `False`, the zero of each numeric type and empty collections[^4]. In JavaScript, "A falsy (sometimes written falsey) value is a value that is considered false when encountered in a Boolean context"[^5], and every other value is truthy. C and C++ treat zero and the null pointer as false and other values as true. In C++, "The value zero (for integral, floating-point, and unscoped enumeration) and the null pointer and the null pointer-to-member values become false. All other values become true"[^6]. Java, C#, Go and Rust have no truthiness: a condition must be a Boolean, so the program writes `x != 0` or `s != null`.

**Booleans and integers.** In some languages a Boolean is a small integer. In Python, "bool is a subclass of int"[^4], so `True + True` is `2`. In C, a comparison produces an `int` with the value `0` or `1`, and the type `bool` holds only those two values. A conversion to `bool` is not a truncation: "(bool)0.5 evaluates to true, whereas (int)0.5 evaluates to 0"[^7]. In other languages the types are separate. Java says that "No other casts on type boolean are allowed"[^2], and C# "provides only two conversions that involve the bool type", both of them to and from `bool?`[^8]. Rust converts with `as` in one direction: "If you cast a bool into an integer, true will be 1 and false will be 0"[^9].

**Storage.** A Boolean needs one bit, but most languages store it in a byte, because a byte is the smallest unit that has an address. `sizeof(bool)` is 1 in C# and on the common C and C++ compilers, and `size_of::<bool>()` is 1 in Rust. Bit sets, bit fields and `std::vector<bool>` pack many Booleans into one word.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `bool`, a subclass of `int`; `True` and `False`; truth values for all objects |
| JavaScript | native | primitive `boolean`; `Boolean()` and `!!`; falsy and truthy values |
| TypeScript | native | `boolean` and the literal types `true` and `false` |
| Java | native | primitive `boolean` and the class `Boolean`; no conversion to or from `int` |
| Kotlin | native | `Boolean`; `if` needs a `Boolean`; `Boolean?` for a nullable value |
| C# | native | `bool` (`System.Boolean`); `bool?` with three-valued `&` and `\|` |
| Go | native | `bool`; zero value `false`; no conversion to or from integers |
| Rust | native | `bool`; `as` converts it to an integer; `then` and `then_some` |
| C | native | C23 makes `bool`, `true` and `false` keywords; comparisons give `int` |
| C++ | native | `bool`; conversions from numbers and pointers; `explicit operator bool` |
| Bash | idiom | the exit status of a command: 0 is success and other values are failure; `true` and `false` are commands |
| PowerShell | native | `[bool]`, `$true` and `$false`; conversion of other values by .NET and PowerShell rules |
| SQL | native | `BOOLEAN` with `TRUE`, `FALSE` and `UNKNOWN`, which is three-valued logic with `NULL` |
| Regex | n/a | a pattern matches or does not match, but there is no Boolean value |
| HTML | idiom | Boolean attributes such as `disabled` and `checked` are true when present and false when absent |
| CSS | n/a | no Boolean values; media queries and `@supports` give true or false conditions |

Every general-purpose language has a Boolean type or an equivalent, so the verdict is native for all of them except Bash, which uses the exit status of commands. The languages differ in whether a condition accepts other types, in whether a Boolean is also an integer, and in how they convert text to a Boolean.

The example files show these differences. Python shows that `True` is `1`, that `1` and `True` are the same dictionary key, how `__bool__` and `__len__` give an object a truth value, and that `and` and `or` return an operand. JavaScript lists all the falsy values and shows that a `Boolean` object is truthy, that `[] == false` is true, and the difference between `||` and `??`. Java shows the `x != 0` idiom, `&` and `&&`, `Boolean.parseBoolean` and a `NullPointerException` from a `null` `Boolean`. C# shows three-valued logic with `bool?`. Go parses Booleans with `strconv` and uses a map to `bool` as a set. Rust converts a `bool` to an integer with `as`, uses `then_some` and `match`, and parses only `true` and `false`. C shows that a comparison gives an `int` and that conversion to `bool` tests for zero. C++ uses `explicit operator bool`, `std::boolalpha` and the proxy of `std::vector<bool>`.

## Variations and related ideas

- **Three-valued logic.** SQL has `TRUE`, `FALSE` and `UNKNOWN`, and C# `bool?` follows the same rules: `null & false` is `false` and `null | true` is `true`, but `null & true` is `null`[^8].
- **Booleans as numbers.** Counting the true values of a list is `sum(flags)` in Python and a filter and `count` in Rust. C, C++ and Python allow arithmetic on Booleans, and Java, C# and Go do not.
- **Option instead of a flag.** A Boolean that says whether another value is valid is often better as an optional value, such as `Option<T>` in Rust. Rust `bool::then_some` turns a condition into an `Option`.
- **Bit sets.** A set of flags fits in the bits of one integer, and `&`, `|` and `^` work on all flags at once.

## Pitfalls

- **Truthiness that surprises.** In Python, `bool("False")` is `True` because the string is not empty. In JavaScript, `"0"` and `[]` are truthy, but `[] == false` is `true`, because `==` converts both sides to numbers.
- **`||` for defaults.** `count || 10` in JavaScript gives `10` when `count` is `0`. Use `??`, which replaces only `null` and `undefined`.
- **Wrapper objects.** A JavaScript `new Boolean(false)` is an object, so it is truthy. MDN says: "Do not use the Boolean() constructor with new"[^10]. A Java `Boolean` can be `null`, and unboxing it in a condition throws `NullPointerException`.
- **Comparing with true.** `if (x == true)` is longer than `if (x)`, and in C it is wrong when `x` is a non-zero value other than 1.
- **Boolean parameters.** A call such as `copy(src, dst, true, false)` does not say what the flags mean. Use named arguments, an enum or two functions.
- **`&` instead of `&&`.** `&` evaluates both operands, so `p != null & p.ok()` in Java throws when `p` is `null`.

## Cost

A Boolean operation is one machine instruction or less, and a condition compiles to a compare and a branch. A branch that the processor cannot predict costs more than the comparison itself, so code in a hot loop sometimes replaces a branch with arithmetic on the 0 or 1 of a Boolean. Storing Booleans as bytes uses eight times the memory of a bit set, which matters only for large arrays.

## History

The Boolean type is named after George Boole, "who first defined an algebraic system of logic in the mid-19th century"[^1]. ALGOL 60 was "One of the earliest programming languages to provide an explicit BOOLEAN data type"[^1]. Its report defines the logical values as `true` and `false` and says that "The logical values have a fixed obvious meaning"[^11]. According to Wikipedia, many later languages adopted this type, among them Simula 67 (1967), ALGOL 68 (1970), Pascal (1970), Ada (1980), Java (1995) and C# (2000)[^1].

FORTRAN at first had no logical values, and "FORTRAN IV (1962), however, follows the ALGOL 60 example by providing a Boolean data type (LOGICAL)"[^1]. C used integers for decades: "Standard C (since C99) provides a Boolean type, called _Bool"[^1], and C23 made `bool`, `true` and `false` keywords. Python added `bool` in 2002 with PEP 285, which defined it as "a straightforward subtype (in C) of the int type"[^12], so old code that used `0` and `1` kept working.

## References
