---
tag: EW24J7
kind: concept
slug: type-system
name: Type system
aliases: [type system, typing discipline, type checker, type checking, type safety, type soundness, strong typing, weak typing, nominal typing, structural typing, duck typing, untyped language, typed language]
area: types/type-systems
level: core
summary: >-
  A type system is a set of rules that gives a type to each part of a program and rejects the programs that break the rules. It finds a class of errors before or while the program runs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Robin Milner]
  note: Milner's paper on type inference for ML gave an algorithm for polymorphic type inference and the first proof of type soundness for a simplified programming language.
see_also: [type, static-and-dynamic-typing, type-conversion, subtyping, hindley-milner, gradual-types, undefined-behavior]
same_as:
  wikidata: Q865760
  wikipedia: Type_system
sources:
- url: http://lucacardelli.name/Papers/TypeSystems.pdf
  title: Luca Cardelli, Type Systems, CRC Handbook of Computer Science and Engineering, 2nd Edition, Chapter 97, 2004
  locator: 1 Introduction; How type systems are formalized; Further Information
- url: https://en.wikipedia.org/wiki/Type_system
  title: Type system, Wikipedia
  locator: Introduction; Static type checking; Dynamic type checking and runtime type information
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: Chapter 4 introduction
- url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html
  title: Type Compatibility, TypeScript Handbook
  locator: Introduction
- url: https://doc.rust-lang.org/book/ch20-01-unsafe-rust.html
  title: Unsafe Rust, The Rust Programming Language
  locator: Introduction; Unsafe Superpowers
- url: https://peps.python.org/pep-0484/
  title: PEP 484, Type Hints
  locator: Rationale and Goals; Non-goals
---

## Idea

A type system is a set of rules that gives a type to each expression of a program, such as `int` to `n + 1` and `bool` to `n < 10`. The rules also say which combinations are wrong, such as adding an integer to a string. A program that breaks a rule is rejected before it runs, or it stops with a type error when it runs. In this way the type system catches a class of mistakes that would otherwise give wrong results without any message.

```python run id=idea
# A small type system: rules give a type to each expression or reject it.
def type_of(e, env):
    match e:
        case bool():
            return "bool"
        case int():
            return "int"
        case str() if e.startswith('"'):
            return "str"
        case str():
            return env[e]  # a variable has the type of its declaration
        case ("+", a, b):
            ta, tb = type_of(a, env), type_of(b, env)
            if ta == tb and ta in ("int", "str"):
                return ta
            raise TypeError(f"cannot add {ta} and {tb}")
        case ("<", a, b):
            if type_of(a, env) == type_of(b, env) == "int":
                return "bool"
            raise TypeError("< needs two ints")
        case ("if", c, a, b):
            if type_of(c, env) != "bool":
                raise TypeError("the condition must be bool")
            ta, tb = type_of(a, env), type_of(b, env)
            if ta != tb:
                raise TypeError(f"the branches have types {ta} and {tb}")
            return ta


env = {"n": "int", "name": "str"}
programs = [
    ("+", "n", 1),
    ("+", "name", '"!"'),
    ("if", ("<", "n", 10), '"small"', '"big"'),
    ("+", "n", '"1"'),
    ("if", "n", 1, 2),
    ("if", True, 1, '"one"'),
]
for p in programs:
    try:
        print("ok   ", type_of(p, env))
    except TypeError as err:
        print("error", err)
```

```output for=idea
ok    int
ok    str
ok    str
error cannot add int and str
error the condition must be bool
error the branches have types int and str
```

`type_of` is a small type checker for a language with numbers, strings, `+`, `<` and `if`. Each `case` is a typing rule: `+` on two `int` values gives an `int`, `<` gives a `bool`, and an `if` needs a `bool` condition and two branches of the same type. The checker finds the type of each program without running it, so it rejects `n + "1"` before any value exists. The last program is `if True then 1 else "one"`, which would run without an error, because the condition is always true. The checker rejects it anyway, because its rules look only at types and not at values.

## How it works

**Purpose.** "The fundamental purpose of a type system is to prevent the occurrence of execution errors during the running of a program"[^1]. Wikipedia describes a simple type system as "a set of rules that associates a data type" with "each term (data-valued expression) in a computer program"[^2]. In more developed type systems, "a variety of constructs, such as variables, expressions, functions, and modules, may be assigned types"[^2].

**Typed and untyped languages.** Cardelli defines a type through the values that a variable can hold: "An upper bound of such a range is called a type of the variable"[^1]. "Languages where variables can be given (nontrivial) types are called typed languages", and "Languages that do not restrict the range of variables are called untyped languages"[^1]. Assembly language is untyped, because each register holds bits that any instruction can use.

**Static and dynamic checking.** "Static type checking is the process of verifying the type safety of a program based on analysis of a program's text (source code)"[^2], and "Dynamic type checking is the process of verifying the type safety of a program at runtime"[^2]. Java uses static checks, and its specification says that "Strong static typing helps detect errors at compile time"[^3]. Python and JavaScript use dynamic checks, so a type error in a line that never runs is never reported. Most statically checked languages also check some things at run time, such as casts, array stores and downcasts. See [static and dynamic typing](/static-and-dynamic-typing/).

**Safety.** Cardelli separates two kinds of execution errors. A trapped error stops the computation, such as a division by zero. An untrapped error goes on with wrong data, such as "accessing data past the end of an array in absence of run time bounds checks"[^1]. "A program fragment is safe if it does not cause untrapped errors to occur. Languages where all program fragments are safe are called safe languages"[^1]. His table puts ML and Java in the typed and safe group, C in the typed and unsafe group, LISP in the untyped and safe group, and assembler in the untyped and unsafe group[^1]. A safe language can be untyped, because it can check each operation at run time. See [undefined behavior](/undefined-behavior/).

**Strong and weak.** The words strong and weak have no single meaning. Cardelli calls a language "weakly checked (or weakly typed, in the literature)" when "some unsafe operations are detected statically and some are not detected"[^1]. In everyday use, a language is also called weakly typed when it converts values of mismatched types, such as JavaScript `1 + "1"`, and strongly typed when it raises an error, such as Python. It is clearer to say which errors a language detects and when.

**Soundness.** A type system is sound when the programs that it accepts do not have the errors that it is meant to prevent. Researchers "attempt to prove a type soundness theorem stating that well-typed programs are well behaved"[^1]. A static checker for a general-purpose language must also reject some correct programs. Wikipedia says that "if a type system is both sound (meaning that it rejects all incorrect programs) and decidable (meaning that it is possible to write an algorithm that determines whether a program is well-typed), then it must be incomplete"[^2]. The last program in the example above is such a case.

**Nominal and structural types.** In a nominal type system, a value has a type only when its declaration says so: a Java class that has an `area()` method is not a `Shape` unless it implements `Shape`. In a structural type system, a value fits a type when it has the right members. "Type compatibility in TypeScript is based on structural subtyping. Structural typing is a way of relating types based solely on their members"[^4]. Go interfaces are structural too. Duck typing is the dynamic form of this idea: Python `len(x)` works for any object with a `__len__` method.

**Ways out of the rules.** Most type systems have a way for the programmer to override them: a cast in C and Java, `dynamic` in C#, `any` in TypeScript, and `unsafe` in Rust and C#. The Rust book says that "Unsafe Rust exists because, by nature, static analysis is conservative"[^5], and that with `unsafe` the programmer tells the compiler, "Trust me, I know what I’m doing"[^5]. Optional type systems go the other way: Python type hints are checked by a separate tool, and "no type checking happens at runtime"[^6].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | dynamic and strong; duck typing; optional hints checked by tools such as mypy |
| JavaScript | native | dynamic and weak; implicit conversions; `TypeError` for calls on wrong values |
| TypeScript | native | static, structural and gradual over JavaScript; `any` and `unknown`; types erased at run time |
| Java | native | static and nominal; checked casts; covariant arrays; erased generics |
| Kotlin | native | static and nominal; nullable types; smart casts; `as?` |
| C# | native | static and nominal; checked casts; `dynamic`; `unsafe` |
| Go | native | static; structural interfaces; no implicit numeric conversions; type assertions |
| Rust | native | static and nominal; traits; no implicit conversions; `unsafe` for raw operations |
| C | native | static and weak; implicit conversions; `void *`; unions; casts |
| C++ | native | static; templates and concepts; `dynamic_cast`; `std::variant` and `std::any` |
| Bash | n/a | values are strings, and there are no types to check |
| PowerShell | native | dynamic with optional type constraints such as `[int]$x`; .NET conversions |
| SQL | native | static types of columns; implicit and explicit `CAST`; rules differ by database |
| Regex | n/a | no types |
| HTML | n/a | no types; a validator checks the document against the content model |
| CSS | idiom | each property has a value grammar, and an invalid value makes the declaration ignored |

Every general-purpose language has a type system. They differ in when they check types, in which errors they detect, in whether they compare types by name or by structure, and in how a program can step outside the rules.

The example files show these differences. Python reports a type error only when the wrong line runs, uses duck typing and a `Protocol`, and ignores annotations. JavaScript converts mismatched values, compares with `==` and `===`, and does not check the arguments of a function. Java shows nominal typing, a checked cast, an `ArrayStoreException` from covariant arrays and a `ClassCastException` that an unchecked generic cast moves to a later read. C# shows a checked cast, array covariance, `dynamic` and `as`. Go satisfies an interface by structure, uses type assertions and keeps types with the same representation apart. Rust checks a trait bound, converts integers with `From` and `TryFrom`, downcasts with `Any` and reads bits in `unsafe` code. C converts values without a cast and reads a `float` through a union. C++ uses a concept, `dynamic_cast`, `std::variant` and `std::any`.

## Variations and related ideas

- **Type inference.** The compiler finds most types without annotations, as in ML, Haskell, Rust and Go `:=`. See [Hindley-Milner type inference](/hindley-milner/).
- **Gradual typing.** TypeScript, Python with type hints and C# with `dynamic` mix static and dynamic checks in one program. See [gradual types](/gradual-types/).
- **Subtyping.** A value of a subtype can be used where a value of its supertype is expected. See [subtyping](/subtyping/).
- **Polymorphism.** Generics, overloading and subtyping let one piece of code work with many types. See [parametric polymorphism](/parametric-polymorphism/).
- **Richer types.** Dependent types put values in types, linear and affine types control how often a value is used, and effect systems track side effects. See [dependent types](/dependent-types/).

## Pitfalls

- **Trusting a cast.** A cast in C tells the compiler to stop checking, and a wrong cast is undefined behavior. In Java and C#, a wrong cast throws an exception only when the program runs.
- **Unsound holes.** Covariant arrays in Java and C#, erased generics in Java, and `any` in TypeScript let a wrong value pass the static check, and the error appears later, far from its cause.
- **Implicit conversions.** C converts `3.99` to `3` and `-1` to a large unsigned value without a cast, and JavaScript converts strings and numbers. Many compilers and linters can warn about these conversions.
- **Annotations that do not run.** Python type hints and TypeScript types do not exist at run time, so data from a file or a network must be checked by the program.
- **Arguing about strong and weak.** These words mean different things to different people. Say instead what the language checks and when it checks it.

## Cost

Static checks cost compile time but nothing at run time, and they let the compiler drop run-time checks and choose fast machine instructions. Wikipedia notes that a compiler that proves a program well-typed "does not need to emit dynamic safety checks, allowing the resulting compiled binary to run faster and to be smaller"[^2]. Dynamic checks cost a test of the type tag at each operation, which JIT compilers reduce by guessing types and checking the guess. Rich static type systems can make compile times long, and C++ templates and TypeScript types can be slow to check.

## History

Early languages such as FORTRAN and ALGOL 60 had a few built-in types and simple rules for them. Pascal, Simula 67 and C added types that a program defines. In 1978, Robin Milner published a paper on type inference for ML. Cardelli writes that it "brought the study of type systems and type inference to a new level", and that "It includes an algorithm for polymorphic type inference, and the first proof of type soundness for a (simplified) programming language"[^1]. Later work added subtyping, generics and type classes, and languages such as Java, C#, Rust, TypeScript and Swift brought these ideas into wide use.

## References
