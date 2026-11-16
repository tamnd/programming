---
tag: 66F7BD
kind: concept
slug: unit-type
name: Unit type
aliases: [unit, '()', Unit, void, Void, None, NoneType, empty tuple, 0-tuple, 'struct{}', 'std::monostate']
area: types/forms-of-types
level: core
summary: >-
  The unit type has exactly one value, so a value of it carries no information. A function that runs only for its side effects returns it, as () in Rust or None in Python.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1968
  people: []
  note: ALGOL 68 had VOID as one of its reserved words, and C and the languages derived from it use void for a function without a result. Functional languages such as ML and Haskell use a real unit type, written ().
see_also: [function-type, composite-type, tuple, side-effect, statements-and-expressions, set-concept, null-references, type]
same_as:
  wikidata: Q1307872
  wikipedia: Unit_type
sources:
- url: https://en.wikipedia.org/wiki/Unit_type
  title: Unit type, Wikipedia
  locator: Introduction; In programming languages; Void type as unit type
- url: https://doc.rust-lang.org/std/primitive.unit.html
  title: Primitive Type unit, The Rust Standard Library
  locator: Description
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
  title: Haskell 2010 Language Report, Chapter 6, Predefined Types and Classes
  locator: 6.1.5 The Unit Datatype
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin/-unit/
  title: Unit, Kotlin Standard Library API reference
  locator: Description
- url: https://docs.python.org/3.14/library/constants.html
  title: Built-in Constants, Python 3.14 documentation
  locator: None
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Size and alignment guarantees
- url: https://en.cppreference.com/w/cpp/utility/variant/monostate.html
  title: std::monostate, C++ reference, cppreference.com
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Void.html
  title: Java SE 25 API, Class Void
  locator: Description
- url: https://en.cppreference.com/w/cpp/language/types.html
  title: Fundamental types, C++ reference, cppreference.com
  locator: void type
- url: https://en.wikipedia.org/wiki/Void_type
  title: Void type, Wikipedia
  locator: Introduction; In C and C++
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/void
  title: void, C# language reference
  locator: Introduction
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Other Types to Know About, void; Return type void
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Mutable Sequence Types, sequence.reverse()
- url: https://en.wikipedia.org/wiki/ALGOL_68
  title: ALGOL 68, Wikipedia
  locator: History; Bold symbols and reserved words
---

## Idea

Some functions run only for what they do: they print a line, write a file or change a variable. They have no result to give back. In a language where each function call is an expression with a type, such a function still needs a result type. The unit type is that type. It has exactly one value, so the value says nothing, and the program can ignore it. Rust and Haskell write the type and its value as `()`, Kotlin calls it `Unit`, and Python uses `None`.

```python run id=idea
# None is the only value of its type.
print(type(None).__name__, None is None, type(None)() is None)


def log(msg: str) -> None:
    print("log:", msg)


result = log("start")
print(result, result is None)


# A function without a return statement returns None.
def nothing():
    pass


print(nothing() is None, [nothing() for _ in range(3)])

# A dict with None values works as a set that keeps the order of insertion.
seen = dict.fromkeys(["b", "a", "b"])
print(seen, list(seen))
```

```output for=idea
NoneType True True
log: start
None True
True [None, None, None]
{'b': None, 'a': None} ['b', 'a']
```

`NoneType` has one value, and even a call to the type gives back the same `None`. `log` has the annotation `-> None`, and its call still has a value that the program can store and test. A function without a `return` statement returns `None` too, so a list of three calls holds three `None` values. In the last lines, the values of the dictionary are all `None`, so only the keys carry information, and the dictionary works as an ordered set.

## How it works

**One value, no information.** "In the area of mathematical logic and computer science known as type theory, a unit type is a type that allows only one value (and thus can hold no information)"[^1]. "One may also regard the unit type as the type of 0-tuples, i.e. the product of no types"[^1]. The unit type is not the empty type: the empty type has no values at all. See [bottom type](/bottom-type/).

**What it is for.** "Several computer programming languages provide a unit type to specify the result type of a function with the sole purpose of causing a side effect, and the argument type of a function that does not require arguments"[^1]. In Rust, "The () type has exactly one value (), and is used when there is no other meaningful value that could be returned", and "functions without a -> ... implicitly have return type ()"[^2]. In Haskell, "The unit datatype () has one non-⊥ member, the nullary constructor ()"[^3].

**The names in each language.** Kotlin has "The type with only one value: the Unit object. This type corresponds to the void type in Java"[^4]. In Python, `None` "is the sole instance of the NoneType type"[^5]. Go has no named unit type, but the empty struct `struct{}` has one value, and "A struct or array type has size zero if it contains no fields (or elements, respectively) that have a size greater than zero"[^6]. The C++ library has `std::monostate`, a "Unit type intended for use as a well-behaved empty alternative in std::variant"[^7]. Java has the class `Void`, which "is an uninstantiable placeholder class to hold a reference to the Class object representing the Java keyword void"[^8]. Its only value is `null`, and code uses it as a type argument, as in `CompletableFuture<Void>`.

**Why expression languages need it.** In Rust, ML and Haskell, a block, an `if` and a function call are expressions, and each expression has a type. A block that ends with a statement still has a value. In Rust, "The semicolon ; can be used to discard the result of an expression at the end of a block, making the expression (and thus the block) evaluate to ()"[^2]. Generic code needs the unit type too: a generic function that returns `T` works for a function without a result only if `T` can be the unit type. Thus `Result<(), E>` in Rust is the result of an operation that can fail but has no value when it succeeds, and `HashMap<K, ()>` is a set. See [statements and expressions](/statements-and-expressions/).

**void is not a full unit type.** C, C++, Java and C# use `void` for a function without a result. In C++, `void` is a "type with an empty set of values. It is an incomplete type that cannot be completed (consequently, objects of type void are disallowed)"[^9]. Wikipedia says that `void` "is semantically similar to an implicit unit type, not a zero or bottom type", but "Unlike a real unit type which is a singleton, the void type lacks a way to represent its value"[^10]. Two limits follow. Wikipedia says that "the void type cannot be the type of an argument in C", so `f(g())` does not compile when `g` returns `void`, and "the void type is special and can never be stored in a record type"[^1]. In C#, `void` is used "as the return type of a method or a local function to specify that the method doesn't return a value"[^11], so a delegate for such a method is an `Action`, not a `Func`. `void *` in C and C++ is another idea: it is a pointer to data of an unknown type, "but this is an unrelated notion"[^10].

**void in TypeScript.** In TypeScript, `void` "represents the return value of functions which" do not return a value[^12]. A function type with a `void` result accepts a function that returns a value: "when implemented, can return any other value, but it will be ignored"[^12]. JavaScript itself returns `undefined` from such a function.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `None`, the one value of `NoneType`; `-> None` in annotations |
| JavaScript | idiom | `undefined` is the result of a function without `return`; `null` also has one value |
| TypeScript | native | `void` for a result that the caller ignores; `undefined` as a type with one value |
| Java | stdlib | `void` for methods; the class `Void`, whose only value is `null`, as a type argument |
| Kotlin | native | `Unit`, a singleton object; a function without a result type returns `Unit` |
| C# | stdlib | `void` for methods; `Action` instead of `Func`; the empty `ValueTuple` as a unit value |
| Go | idiom | `struct{}` with the one value `struct{}{}`, of size zero; a function can have no result |
| Rust | native | `()`, the empty tuple; a block that ends with `;` has the value `()` |
| C | idiom | `void` for a function without a result or without parameters; no unit value |
| C++ | native | `void` as in C; `std::monostate` and an empty struct are unit types |
| Bash | idiom | a function always gives an exit status; output, not a return value, carries data |
| PowerShell | idiom | `[void]` discards a value; a function with no output returns nothing, which is `$null` when stored |
| SQL | idiom | a procedure has no result value; `NULL` stands for an absent value |
| Regex | n/a | a pattern has no values |
| HTML | n/a | HTML has no types of values |
| CSS | n/a | CSS has no unit type; its units, such as `px`, are units of measure |

The table has the verdict "stdlib" for Java and C#, because `void` covers the result of a method but is not a type that generic code can use, so these languages use library types such as `Void`, `Action` and `ValueTuple`. Rust, Kotlin, Python and C++ with `std::monostate` have a real unit type. Go and C have no named unit type.

The example files show these differences. Python shows that `list.sort` returns `None` and that `None` is falsy. JavaScript shows that a function without `return` gives `undefined` and that the `void` operator gives `undefined` too. Java uses `Void` as a type argument and shows that `Runnable` and `Supplier` are two interfaces. C# uses `Action` for a method without a result and shows the empty `ValueTuple`. Go uses `map[string]struct{}` as a set and a channel of `struct{}` as a signal. Rust shows that `()` has the size 0, that `;` gives `()`, and uses `HashMap<K, ()>` and `Result<(), E>`. C shows `void` functions and the `(void)` cast that discards a result. C++ shows `std::monostate` in a `std::variant` and `std::is_void`.

## Variations and related ideas

- **Unit versus bottom.** The unit type has one value, and the [bottom type](/bottom-type/) has none. A function that returns unit returns normally. A function that returns the bottom type does not return.
- **Unit versus null.** `None` in Python and `null` in Java are also the values that stand for a missing value. The unit type itself has no notion of absence, but in these languages one value does both jobs. See [null references](/null-references/).
- **Empty tuple and empty struct.** The unit type is the product of no types, so Rust writes it as the empty tuple, and Go and C++ can use an empty struct. See [tuple](/tuple/) and [composite type](/composite-type/).
- **A set as a map to unit.** `HashMap<K, ()>` in Rust and `map[K]struct{}` in Go keep only the keys. See [set](/set-concept/).

## Pitfalls

- **A method that changes in place returns unit.** In Python, `x = items.sort()` sets `x` to `None`. The documentation says that, "To remind users that it operates by side-effect, it returns None"[^13]. Use `sorted(items)` for a sorted copy.
- **Generic code that cannot use void.** `Func<void>` does not exist in C#, and `Function<String, void>` does not compile in Java. Write two overloads, one with `Action` and one with `Func`, or use `Void` and return `null` in Java.
- **A TypeScript callback that returns a value.** Because a `void` function type accepts a function that returns a value[^12], `forEach` accepts `push` as a callback, and the result of `push` is dropped. Do not use the result of a call whose type is `void`.
- **A void result in C.** A call to a `void` function cannot be an argument or an operand, because "the void type cannot be the type of an argument in C"[^1].
- **Unit is not a missing value.** In Python and JavaScript, `None` and `undefined` are both the result of a function without a result and a marker for a missing value. A dictionary lookup that returns `None` can mean "absent" or "present with the value None".

## Cost

A value of the unit type carries no information, so it needs no storage and no work. In Rust, `()` and a struct of units have the size 0, and in Go, a value of size zero can share its address with another one[^6]. A `HashMap<K, ()>` stores only the keys, so it costs the same as a set. In Python, `None` is one object that all code shares, and a `dict` with `None` values still stores a pointer for each value. In Java, `Void` costs nothing, because its only value is `null`.

## History

The void type is older than the name unit in most languages. ALGOL 68, which IFIP Working Group 2.1 adopted "On 20 December 1968"[^14], lists `VOID` among its reserved words[^14]. Wikipedia says that the void type appears in "curly bracket programming languages derived from C and ALGOL 68"[^10]. "In the earliest versions of C, functions with no specific result defaulted to a return type of int"[^10], and `void` came later. The functional languages took the unit type from type theory: "In Haskell, Rust, and Elm, the unit type is called () and its only value is also ()"[^1], and "In ML descendants (including OCaml, Standard ML, and F#), the type is called unit but the value is written as ()"[^1].

## References
