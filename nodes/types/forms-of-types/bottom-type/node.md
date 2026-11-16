---
tag: 249PFT
kind: concept
slug: bottom-type
name: Bottom type
aliases: [never type, empty type, uninhabited type, zero type, Never, NoReturn, never, Nothing, '!', Void, '⊥']
area: types/forms-of-types
level: advanced
summary: >-
  The bottom type has no values and is a subtype of every type. It is the result type of code that never returns, such as a throw, a panic or an endless loop.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1997
  people: [Benjamin C. Pierce]
  note: In his 1997 report Bounded Quantification with Bottom, Pierce listed the uses of a bottom type in a language with subtyping. Rust, Scala, Kotlin, TypeScript and Python each have a named bottom type.
see_also: [unit-type, subtyping, exhaustive-matching, panics, exception-handling, infinite-loops, null-references, type]
same_as:
  wikidata: Q3546108
  wikipedia: Bottom_type
sources:
- url: https://en.wikipedia.org/wiki/Bottom_type
  title: Bottom type, Wikipedia
  locator: Introduction; Relation with the empty type; Computer science applications; In programming languages
- url: https://doc.rust-lang.org/reference/types/never.html
  title: Never type, The Rust Reference
  locator: type.never.intro; type.never.coercion
- url: https://doc.rust-lang.org/std/primitive.never.html
  title: Primitive Type never, The Rust Standard Library
  locator: Description; Never-to-any coercion
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: typing.Never; typing.NoReturn; typing.assert_never
- url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html
  title: Narrowing, The TypeScript Handbook
  locator: The never type; Exhaustiveness checking
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin/-nothing/
  title: Nothing, Kotlin Standard Library API reference
  locator: Description
- url: https://hackage.haskell.org/package/base-4.21.0.0/docs/Data-Void.html
  title: Data.Void, base 4.21.0.0, Hackage
  locator: Description
- url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-2-0.html
  title: TypeScript 2.0 release notes
  locator: The never type
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/exception-handling-statements
  title: Exception-handling statements, C# language reference
  locator: The throw expression
- url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.codeanalysis.doesnotreturnattribute?view=net-10.0
  title: DoesNotReturnAttribute Class, .NET API reference
  locator: Definition
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.1 The Kinds of Types and Values
- url: https://en.cppreference.com/w/cpp/language/attributes/noreturn.html
  title: C++ attribute noreturn, C++ reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/c/language/noreturn.html
  title: _Noreturn function specifier, C reference, cppreference.com
  locator: Explanation; Standard library
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Terminating statements
---

## Idea

Some expressions never give a value. A `raise`, a call to `exit` and an endless loop all stop the normal flow, so the code after them does not run. The bottom type is the type of such an expression. It has no values, so a function whose result type is the bottom type can never return normally. Because no value of it can exist, the bottom type fits where any other type is expected, which lets a branch that fails sit next to a branch that gives a value. Python calls it `Never`, Rust writes `!`, Kotlin has `Nothing`, and TypeScript has `never`.

```python run id=idea
from typing import Never, NoReturn, assert_never


def fail(msg: str) -> Never:
    raise ValueError(msg)


def parse_age(s: str) -> int:
    # Each branch must give an int. fail gives no value, so it fits here.
    return int(s) if s.isdigit() else fail(f"bad age: {s!r}")


print(parse_age("42"))
try:
    parse_age("x")
except ValueError as e:
    print("error:", e)


def describe(x: int | str) -> str:
    if isinstance(x, int):
        return "int"
    elif isinstance(x, str):
        return "str"
    else:
        # A type checker narrows x to Never here, so this line is unreachable.
        assert_never(x)


print(describe(1), describe("a"))
try:
    describe(1.5)  # type: ignore[arg-type]
except AssertionError as e:
    print(type(e).__name__)
print(Never, NoReturn)
```

```output for=idea
42
error: bad age: 'x'
int str
AssertionError
typing.Never typing.NoReturn
```

`fail` has the result type `Never`, so a type checker accepts it in the `else` branch of an expression whose type is `int`. In `describe`, the first two branches cover every value of `int | str`, so a type checker gives `x` the type `Never` in the last branch, and `assert_never` confirms that this branch is unreachable. Python does not check annotations when the program runs, so the call with `1.5` reaches `assert_never`, which raises an `AssertionError`.

## How it works

**A subtype of all types.** "In type theory, a theory within mathematical logic, the bottom type of a type system is the type that is a subtype of all other types"[^1]. "Where such a type exists, it is often represented with the up tack (⊥) symbol"[^1]. It is the dual of the [top type](/top-type/), the type that every value has.

**No values.** "When the bottom type is uninhabited, a function whose return type is bottom cannot return any value, not even the lone value of a unit type"[^1]. That is the difference from the [unit type](/unit-type/): a function that returns unit returns normally with a value that carries no information, and a function that returns bottom does not return at all. In Rust, "The never type ! is a type with no values, representing computations that never complete, also known as diverging computations"[^2].

**Why it fits anywhere.** A value of the bottom type can never exist, so a rule that lets such a value become any other type can never go wrong at run time. The Rust reference says, "Expressions of type ! can be coerced into any type"[^2], and the standard library explains, "This is sound because a value of type ! can never exist, and any coercion of such a value will never actually execute"[^3]. This is how a `match` arm or an `if` branch can end in `return`, `panic!` or `continue` while the other arms give values. In Rust, "return, break, continue, become, and infinite loop expressions all have type !, as the code following them is unreachable"[^3].

**Exhaustiveness and unreachable code.** After a program has tested a value against every case of a union, no case is left, and the type of the value in the remaining branch is the bottom type. In Python, "For a call to assert_never to pass type checking, the inferred type of the argument passed in must be the bottom type, Never, and nothing else"[^4]. A type checker reports an error if a case is missing, so the bottom type turns a missing case into a compile-time error. The TypeScript handbook says the same of `never`: "you can use narrowing and rely on never turning up to do exhaustive checking in a switch statement"[^5]. See [exhaustive matching](/exhaustive-matching/).

**A type with no values as a parameter.** A generic type with the bottom type as its argument can rule out a case. In Rust, "the Err variant of a Result<T, !> cannot be constructed either"[^3], so `Result<T, !>` is the result of an operation that cannot fail. A function that takes the bottom type as a parameter cannot be called: Python documents `Never` as the parameter type of "a function that should never be called, as there are no valid arguments"[^4].

**The names in each language.** In Python, "Never and NoReturn represent the bottom type, a type that has no members"[^4]. Kotlin says that "Nothing has no instances" and that "if a function has the return type of Nothing, it means that it never returns (always throws an exception)"[^6]. Haskell has `Void` in `Data.Void`, "A logically uninhabited data type, used to indicate that a given term should not exist"[^7]. In TypeScript, "The never type represents the type of values that never occur"[^8], and "never is a subtype of and assignable to every type"[^8].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `typing.Never` and `typing.NoReturn`, checked by a type checker, not at run time |
| JavaScript | absent | no static types; `throw` is a statement, so a helper function that throws stands in |
| TypeScript | native | `never`, inferred for a function that cannot reach its end and for a union with no cases left |
| Java | absent | no bottom type; a method that always throws declares some result type; the null type is not a bottom type |
| Kotlin | native | `Nothing`, which has no instances; a function that always throws has the result type `Nothing` |
| C# | idiom | no bottom type; `throw` expressions fit where a value is expected; `[DoesNotReturn]` for analysis |
| Go | idiom | no bottom type; a call to `panic` is a terminating statement, so no `return` must follow it |
| Rust | native | `!`, the type of `panic!`, `return`, `break`, `continue` and `loop`; `Infallible` for errors that cannot occur |
| C | idiom | no bottom type; `_Noreturn` or `[[noreturn]]` marks a function that does not return, such as `exit` |
| C++ | idiom | no bottom type; `[[noreturn]]` on a function; a `throw` expression in a conditional expression |
| Bash | n/a | Bash has no static types; `exit` ends the script |
| PowerShell | absent | no bottom type; `throw` and `exit` stop the normal flow |
| SQL | n/a | SQL has no result type for code that does not return |
| Regex | n/a | a pattern has no values |
| HTML | n/a | HTML has no types of values |
| CSS | n/a | CSS has no types of values in this sense |

Rust, Kotlin and TypeScript have a bottom type in the language, and the compiler uses it to type `return`, `throw` and loops. Python has one in its type annotations, for type checkers. In C, C++, C# and Go, only an attribute or a rule about statements says that a call does not return, and no expression has a bottom type. In C#, "You can also use throw as an expression"[^9], so `name ?? throw ...` fits where a string is expected, and the attribute `DoesNotReturn` "Specifies that a method will never return under any circumstance"[^10]. Java has a null type, which is a subtype of every reference type, but "the null type is not a bottom type as described above, it is not a subtype of int and other primitive types"[^1]. Also, "Because the null type has no name, it is impossible to declare a variable of the null type"[^11].

The example files show these differences. Python shows `Never` on a function that calls `sys.exit` and shows that Python does not check the annotation. JavaScript uses a helper function that throws, because `throw` is a statement. Java uses a generic method that throws to fill a branch of a conditional expression and a `switch` arm. C# uses `throw` expressions with `??` and the conditional operator. Go shows a function that ends in `panic` without `return`. Rust shows a function that returns `!`, `continue` and `return` in a `match`, and `Infallible`. C and C++ show `noreturn` functions.

## Variations and related ideas

- **Bottom versus unit.** The [unit type](/unit-type/) has one value, so a function that returns it returns. The bottom type has none, so a function that returns it does not.
- **Inhabited bottom types.** "However, when the bottom type is inhabited, it is then different from the empty type"[^1].
- **Uninhabited types in general.** An enum with no variants, such as `enum Void {}` in Rust, also has no values. The Rust standard library says that a variant with such a field "can't be created and thus doesn't have to be matched"[^3].
- **The null type.** In Java, null has a type that is a subtype of all reference types. Wikipedia lists the type of the null pointer as one use of a bottom type[^1]. See [null references](/null-references/).
- **Attributes instead of a type.** C and C++ mark a function with `noreturn`. The C++ attribute "Indicates that the function will not return control flow to the calling function after it finishes (e.g. functions that terminate the application, throw exceptions, loop indefinitely, etc.)"[^12].

## Pitfalls

- **A noreturn function that returns.** In C, "If the function declared _Noreturn returns, the behavior is undefined"[^13]. In C++, "If a function previously declared with [[noreturn]] is invoked and that invocation eventually returns, the behavior is runtime-undefined"[^12].
- **Python does not check Never.** A function with the annotation `-> Never` can return `None` when the program runs. Only a type checker reports it, as the Python example shows.
- **never in a union disappears.** In TypeScript, "Because never is a subtype of every type, it is always omitted from union types"[^8], so `string | never` is `string`.
- **A helper that never returns, in Go.** The Go specification lists "A call to the built-in function panic" among the terminating statements[^14]. A call to your own function that always panics, or to `os.Exit`, is not on that list, so a function with a result still needs a `return` after it. Call `panic` directly at the end, as the Go example does.
- **void is not bottom.** A function that returns `void` in C, C++, Java or C# returns normally. Do not use `void` for a function that never returns. Use the attribute or the bottom type of the language.

## Cost

The bottom type costs nothing when the program runs, because no value of it ever exists and no code after a diverging expression runs. In C and C++, `noreturn` is a promise to the compiler, and a program that breaks the promise has undefined behavior[^13][^12]. In Rust, `Result<u32, Infallible>` takes no more space than `u32`, as the Rust example shows, and the compiler does not "require you to handle the Err case"[^3].

## History

Pierce's 1997 report Bounded Quantification with Bottom listed the uses of a bottom type, for example "a natural type for the raise construct is raise ∈ exception -> Bot" and "List(Bot) is a good type for nil"[^1]. In Haskell, the type is called `Void`, and in Scala and Kotlin `Nothing`[^1]. TypeScript 2.0 added `never`[^8]. Python has `typing.NoReturn` and, from Python 3.11, `typing.Never`, which "have the same meaning in the type system"[^4]. C11 added the `_Noreturn` specifier, and C23 deprecated it in favor of `[[noreturn]]`[^13]. C++11 added the `[[noreturn]]` attribute[^12].

## References
