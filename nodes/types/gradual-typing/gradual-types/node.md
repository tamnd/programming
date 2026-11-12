---
tag: NEW-gradual-types
kind: concept
slug: gradual-types
name: Gradual typing
aliases: [gradual typing, optional typing, type hints, the dynamic type, any type, unknown type, consistency, casts, blame, migratory typing, sound gradual typing]
area: types/gradual-typing
level: core
summary: >-
  Gradual typing lets one program mix code with static types and code without them. An unknown type matches every type, and checks at the boundary catch values of the wrong type when the program runs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2006
  people: [Jeremy Siek, Walid Taha, Sam Tobin-Hochstadt, Matthias Felleisen]
  note: Siek and Taha named gradual typing in 2006, the same year that Tobin-Hochstadt and Felleisen described the migration from untyped to typed Scheme code.
see_also: [static-and-dynamic-typing, type-conversion, subtyping, type-erasure, assertions, hindley-milner, sum-types]
same_as:
  wikidata: Q16965428
  wikipedia: Gradual_typing
sources:
- url: https://docs.python.org/3/library/typing.html#the-any-type
  title: typing, Support for type hints, Python documentation
  locator: The Any type
- url: https://wphomes.soic.indiana.edu/jsiek/
  title: Jeremy G. Siek, home page, Indiana University
  locator: Introduction and Technology Transfer
- url: https://peps.python.org/pep-0484/
  title: PEP 484, Type Hints
  locator: Rationale and goals
- url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#any
  title: Everyday Types, The TypeScript Handbook
  locator: any and noImplicitAny
- url: https://learn.microsoft.com/en-us/dotnet/csharp/advanced-topics/interop/using-type-dynamic
  title: Using type dynamic, C# documentation
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/utility/any.html
  title: std::any, cppreference.com
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.12.2
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25 Edition
  locator: 4.12.2 Variables of Reference Type
---

## Idea

Gradual typing lets one program mix code that has static types with code that does not. Code without annotations gets an unknown type, often written `?`, `Any` or `dynamic`, and the checker lets a value of the unknown type go wherever a typed value can go. Where a value moves from unknown to known types, a sound gradual system inserts a check that runs when the program runs, so a value of the wrong type fails at the boundary, with the blame on the untyped side. A team can then add types to a large program one module at a time. TypeScript, Python's type hints, PHP's types, C#'s `dynamic` and Racket's typed modules all follow this idea, with different degrees of checking.

```python run id=idea
DYN = "?"  # the unknown type of gradual typing


def consistent(a, b):  # like equality, but "?" matches every type
    if DYN in (a, b):
        return True
    if isinstance(a, tuple) and isinstance(b, tuple):  # function types
        return len(a) == len(b) and all(map(consistent, a, b))
    return a == b


def check_call(fn_type, arg_type):  # the static check at a call
    param, result = fn_type
    if not consistent(param, arg_type):
        return f"static error: {arg_type} is not consistent with {param}"
    if param != arg_type:  # a cast is needed where "?" meets a known type
        return f"ok, with a run-time cast from {arg_type} to {param}"
    return "ok, no cast"


def cast(value, typ, blame):  # the run-time check that the cast inserts
    if typ != DYN and type(value).__name__ != typ:
        raise TypeError(f"{blame}: expected {typ}, got {type(value).__name__}")
    return value


print(consistent("int", DYN), consistent(("int", "str"), (DYN, "str")),
      consistent("int", "str"))
double = ("int", "int")
for arg in ["int", DYN, "str"]:
    print(f"double(<{arg}>):", check_call(double, arg))

for value in [21, "21"]:  # a value of type "?" reaches the typed function
    try:
        print("double:", 2 * cast(value, "int", "the caller of double"))
    except TypeError as err:
        print("blame ->", err)
```

```output for=idea
True True False
double(<int>): ok, no cast
double(<?>): ok, with a run-time cast from ? to int
double(<str>): static error: str is not consistent with int
double: 42
blame -> the caller of double: expected int, got str
```

The checker uses consistency in place of equality: `?` is consistent with every type, and two function types are consistent when their parts are. A call with an `int` needs no check, a call with a `str` is a static error, and a call with a `?` passes the static check but gets a cast. The cast lets 21 through and stops `"21"` at the boundary, with a message that blames the untyped code that supplied it.

## How it works

**The unknown type.** A gradual type system adds one type, the unknown or dynamic type, to a static type system. Python's documentation describes its rules: "A static type checker will treat every type as assignable to Any and Any as assignable to every type."[^1] Code that has no annotations is treated as if every name had this type, so the checker accepts it as it is. Jeremy Siek, who invented the approach "with Walid Taha", describes it as "mixing static and dynamic type checking within the same language".[^2]

**Consistency, not subtyping.** The unknown type cannot be a normal supertype of every type, because then every type would be a subtype of every other through it, and the checker would accept anything. Gradual typing uses a separate relation, consistency, which is not transitive: `int` is consistent with `?` and `?` with `str`, but `int` is not consistent with `str`. The checker still rejects errors between known types, as the example shows.

**Casts and blame.** In a sound gradual system the compiler inserts a cast wherever a value of the unknown type flows into a known type, and the reverse for functions and objects that cross the boundary. A failed cast stops the program at the boundary, not deep inside the typed code, and blame tracking names the side that broke the agreement. The result is a guarantee: typed code never sees a value of the wrong type, and any error is the fault of untyped code. Typed Racket gives this guarantee with contracts at module boundaries.

**Optional typing.** Most gradual systems in wide use leave out the casts. TypeScript removes all types when it compiles to JavaScript, and PEP 484 states for Python that "no type checking happens at runtime".[^3] These systems find errors in typed code before it runs, but a wrong value from untyped code, a JSON file or a cast travels on and fails later, or not at all. The trade gives no run-time cost and easy adoption for a weaker guarantee, which the community calls optional or unsound gradual typing. PEP 484 also states the aim: "Python will remain a dynamically typed language, and the authors have no desire to ever make type hints mandatory, even by convention."[^3]

**Migration.** The practical use of gradual typing is migration. A team adds annotations to the modules where errors cost the most, turns on stricter checks file by file, and ends with most code typed. TypeScript's `noImplicitAny` option marks each place where the type silently became `any`, because "any isn’t type-checked".[^4] Mypy and Pyright have similar strict modes. By default mypy does not check the body of a function that has no annotations.

**Static languages with a dynamic type.** Statically typed languages approach the same mix from the other side. C# has `dynamic`, and "an object of type dynamic bypasses static type checking".[^5] Java, Go, Rust and C++ have a type that holds any value, `Object`, `any`, `dyn Any` and `std::any`, together with casts or downcasts that check the type when they run. The C++ standard library describes `std::any` as "a type-safe container for single values of any copy constructible type".[^6] These are checked dynamic typing inside a static language; they do not let a program leave out types.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | annotations, `typing.Any`, mypy and Pyright; no checks at run time |
| JavaScript | absent | no annotations; TypeScript and Flow add them |
| TypeScript | native | `any` and `unknown`; types removed before the code runs |
| Java | idiom | `Object`, checked casts, `instanceof`, type patterns |
| Kotlin | idiom | `Any`, `is` and `as?`; `dynamic` on the JavaScript target |
| C# | native | the `dynamic` type and the run-time binder |
| Go | idiom | `any`, type assertions and type switches |
| Rust | stdlib | `std::any::Any` and downcasts |
| C | idiom | tagged unions; `void *` with no check |
| C++ | stdlib | `std::any`, `std::any_cast`, `std::variant` |
| Bash | n/a | every value is a string |
| PowerShell | native | untyped variables, or types that constrain them |
| SQL | n/a | declared column types |
| Regex | n/a | no types |
| HTML | n/a | no types |
| CSS | n/a | no types |

Python, TypeScript, C# and PowerShell let one program choose, name by name, between static and dynamic checking, so they are native. Python and TypeScript start from a dynamic language and add optional static types that do not run; C# starts from a static language and adds a dynamic type whose operations the run-time binder checks. PowerShell checks types when it runs, and only for variables that declare one.

Java, Go, Rust, C and C++ are static languages with one escape: a type that holds any value, and a checked way to get the real type back. The example files show each form: the checked casts and type patterns of Java, Go's type switch over decoded JSON, Rust's downcasts, C's tagged union and C++'s `std::any`. The JavaScript file writes contracts with blame by hand, and the Python file shows that annotations are not checked unless code checks them at the boundary.

## Variations and related ideas

- **Sound gradual typing.** Typed Racket, Reticulated Python and Grace insert casts at every boundary and keep the guarantee that typed code sees only values of its types.
- **Transient checks.** A cheaper form, used by Reticulated Python, checks only the outer shape of a value, such as "is a list", where it enters typed code, and checks the parts when typed code reads them.
- **Optional typing.** TypeScript, Flow, mypy, Pyright, Sorbet for Ruby and Hack's original mode check types before the program runs and remove them. Dart 1 was optionally typed; Dart 2 made its types sound. See [static and dynamic typing](/static-and-dynamic-typing/).
- **Concrete types.** Some research designs, such as Thorn, StrongScript and Nom, check types at run time only for classes, which makes the checks fast.
- **The unknown type in TypeScript.** `unknown` holds any value but allows no operation until code checks its type, a safer form of `any`.
- **Type inference.** Inference fills in types without annotations, and gradual languages use it to reduce the code that falls back to the unknown type. See [Hindley-Milner](/hindley-milner/).
- **Contracts.** Racket's contracts and design by contract check values at boundaries with any predicate, not only types. See [assertions](/assertions/).

## Pitfalls

- **Any spreads.** An operation on a value of type `Any` gives `Any`, so one unannotated function can turn a whole chain of expressions into unchecked code. Turn on `noImplicitAny` in TypeScript and the `disallow_any_generics` option and strict mode in mypy, and prefer `unknown` or `object` when the type is truly not known.
- **Types that lie.** In an optional system, an annotation is a claim, not a check. JSON, user input and calls from untyped code can give values that do not match, and the error appears far from the cause. Validate data where it enters the program.
- **Raw and unchecked types.** Java's raw types and unchecked casts let a value of the wrong type into a generic collection; the JLS calls the result heap pollution.[^7] The wrong block in the Java file shows the late failure. Treat unchecked warnings as errors.
- **Casts with no check.** TypeScript's `as`, C's casts and `static_cast` in C++ tell the compiler to trust the programmer, and nothing checks them when the program runs.
- **The cost of sound checks.** Casts on functions and objects wrap them in proxies, and a value that crosses many boundaries can collect many wrappers. Programs in sound gradual systems have run many times slower when typed and untyped modules call each other in a loop.

## Cost

Optional systems such as TypeScript and Python's type hints cost nothing at run time, because the types are removed or ignored. A dynamic type in a static language, such as C#'s `dynamic`, costs a call to the run-time binder for each operation, which caches the result for each call site after the first call. `std::any` puts values larger than a small buffer on the heap, `Box<dyn Any>` always does, and each cast compares a type identifier. Sound gradual systems pay for a check at each boundary and for a wrapper around each function or object that crosses it; the cost depends on how often values cross, and it was the main research problem of the field after 2015.

## History

Lisp compilers accepted optional type declarations from the 1970s, and Common Lisp, from 1984, and Dylan used them to generate faster code. Erik Meijer and Peter Drayton argued in 2004 for static typing where possible and dynamic typing where needed. Jeremy Siek and Walid Taha named gradual typing and defined consistency in a 2006 paper,[^2] and in the same year Sam Tobin-Hochstadt and Matthias Felleisen described the migration from untyped to typed Scheme code, the work that became Typed Racket. Philip Wadler and Robert Bruce Findler added blame to gradual types in 2009.

Industry adopted the idea from 2010. C# 4.0 added `dynamic` in 2010.[^5] Microsoft released TypeScript in 2012, which Siek describes as "a gradually-typed dialect of JavaScript".[^2] Facebook released Hack, a typed dialect of PHP, and Flow for JavaScript in 2014, and PEP 484 added type hints to Python 3.5 in 2015.[^3] Research then turned to the cost of sound checks, after a 2016 study found large slowdowns in Typed Racket programs that mixed typed and untyped modules.

## References
