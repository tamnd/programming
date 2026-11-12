---
tag: NEW-dependent-types
kind: concept
slug: dependent-types
name: Dependent and refinement types
aliases: [dependent types, dependent type, refinement types, refinement type, liquid types, length-indexed vectors, pi types, sigma types, subrange types, const generics]
area: types/dependent-and-refinement-types
level: core
summary: >-
  A dependent type depends on a value, such as a vector type that includes its length. A refinement type adds a predicate, such as 0 to 100. Both move checks from run time to compile time.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1972
  people: [Per Martin-Löf, Thierry Coquand, Tim Freeman, Frank Pfenning, Hongwei Xi, Ranjit Jhala]
  note: Per Martin-Löf presented intuitionistic type theory, with dependent types, in 1972. Freeman and Pfenning introduced refinement types for ML in 1991.
see_also: [static-and-dynamic-typing, parametric-polymorphism, out-of-bounds-read, sum-types, assertions, gradual-types, hindley-milner, type-conversion]
same_as:
  wikidata: Q997433
  wikipedia: Dependent_type
sources:
- url: https://docs.idris-lang.org/en/latest/tutorial/typesfuns.html
  title: Types and Functions, Idris 2 documentation
  locator: Dependent Types, Vectors
- url: https://agda.readthedocs.io/en/latest/getting-started/what-is-agda.html
  title: What is Agda?, Agda documentation
  locator: Introduction
- url: https://ucsd-progsys.github.io/liquidhaskell/
  title: LiquidHaskell, home page
  locator: Introduction
- url: https://doc.rust-lang.org/reference/items/generics.html
  title: Generic parameters, The Rust Reference
  locator: Const generics
- url: https://en.cppreference.com/w/cpp/language/constraints.html
  title: Constraints and concepts, cppreference.com
  locator: Concepts
- url: https://en.cppreference.com/w/c/language/array.html
  title: Array declaration, cppreference.com
  locator: Variable-length arrays
- url: https://doc.rust-lang.org/std/num/struct.NonZero.html
  title: Struct NonZero, Rust standard library documentation
  locator: Description
- url: https://www.postgresql.org/docs/current/sql-createdomain.html
  title: CREATE DOMAIN, PostgreSQL documentation
  locator: Description
- url: https://docs.python.org/3/library/typing.html#typing.Annotated
  title: typing, Support for type hints, Python documentation
  locator: typing.Annotated
---

## Idea

A dependent type is a type that depends on a value. The type `Vect 3 Int`, a list of exactly three integers, includes the number 3, and a function can say that it returns `Vect (n + m) a` when it joins a `Vect n a` and a `Vect m a`. A refinement type is a base type with a predicate, such as `{v: Int | 0 <= v < n}`, the integers that are valid indexes of a list of length `n`. Both kinds of type let the compiler check facts about values, such as lengths, ranges and orders, that ordinary types leave to tests and run-time checks. Full dependent types are found in proof assistants and research languages such as Agda, Rocq, Lean and Idris. Mainstream languages have weaker forms: array lengths in types, const generics, validated wrapper types and range constraints.

```python run id=idea
# Refinement types: a base type plus a predicate, as in {v: int | 0 <= v < n}
def check(facts, need, names):
    """Prove that the facts imply the need, or return a counterexample.
    A search over small values stands in for the SMT solver."""
    for vals in ((i, n) for i in range(-1, 9) for n in range(0, 9)):
        env = dict(zip(names, vals))
        if all(f(**env) for f in facts) and not need(**env):
            return "rejected, counterexample " + ", ".join(
                f"{k} = {v}" for k, v in env.items())
    return "accepted"


in_bounds = lambda i, n: 0 <= i < n  # what xs[i] needs, with n = len(xs) >= 0
facts = {
    "no check": [],
    "if i >= 0": [lambda i, n: i >= 0],
    "if 0 <= i < n": [lambda i, n: 0 <= i < n],
}
for label, fs in facts.items():
    print(f"xs[i] under {label}:", check(fs, in_bounds, ("i", "n")))


# Dependent types: the type of a result can depend on a value
def vec(n):  # Vect n: a list whose length is part of its type
    return ("Vect", n)


def append(t, u):  # (++) : Vect n -> Vect m -> Vect (n + m)
    return vec(t[1] + u[1])


def zip_type(t, u):  # zip : Vect n -> Vect n -> Vect n
    if t != u:
        return f"type error: {t} and {u} differ"
    return t


ab = append(vec(2), vec(3))
print("append(Vect 2, Vect 3) :", ab)
print("zip with Vect 5        :", zip_type(ab, vec(5)))
print("zip with Vect 4        :", zip_type(ab, vec(4)))
```

```output for=idea
xs[i] under no check: rejected, counterexample i = -1, n = 0
xs[i] under if i >= 0: rejected, counterexample i = 0, n = 0
xs[i] under if 0 <= i < n: accepted
append(Vect 2, Vect 3) : ('Vect', 5)
zip with Vect 5        : ('Vect', 5)
zip with Vect 4        : type error: ('Vect', 5) and ('Vect', 4) differ
```

The first part checks the index expression `xs[i]` as a refinement type checker does. The access needs `0 <= i < n`, and the checker must prove it from the facts that hold at that point in the program, which come from the conditions of the enclosing `if` statements. With no facts, or with only `i >= 0`, the checker finds a counterexample: an empty list. With both bounds it accepts the access, and the program needs no bounds check when it runs. The second part computes types from values: the type of `append` adds the two lengths, and `zip` accepts two vectors only when their length types are equal.

## How it works

**Types that depend on values.** In a dependently typed language, types are values. Idris says so directly: "In Idris, types are first class, meaning that they can be computed and manipulated (and passed to functions) just like any other language construct."[^1] A family of types such as `Vect n a` is a function from a number and a type to a type. A dependent function type, often written `(n : Nat) -> Vect n a`, describes a function whose result type depends on the value of its argument. A dependent pair, such as a number `n` together with a `Vect n a`, holds a value whose type depends on the first part. The Idris tutorial shows what this gives: the type of append "states that the resulting vector’s length will be the sum of the input lengths", and Idris rejects a definition that breaks this.[^1]

**Checking needs evaluation.** To check that `Vect (2 + 3) Int` and `Vect 5 Int` are the same type, the compiler must run `2 + 3`. A type checker for dependent types therefore contains an evaluator, and it compares types after it reduces them. Because a function that never ends could make type checking never end, most dependently typed languages require that functions used in types are total: they end and handle every input. Values that are known only at run time stay as variables, and the checker reasons about them with the rules for each function, for example that `0 + m` reduces to `m`.

**Propositions as types.** A dependent type can state any property, such as that a list is sorted or that two values are equal, and a value of that type is a proof of the property. This correspondence makes dependently typed languages into proof assistants: "Because of strong typing and dependent types, Agda can be used as a proof assistant, allowing one to prove mathematical theorems (in a constructive setting) and to run such proofs as algorithms."[^2] Rocq, Lean and Agda are used to check proofs in mathematics and to verify compilers and operating system kernels.

**Refinement types.** A refinement type keeps the types of an ordinary language and adds a predicate from a logic that a solver can decide, usually linear arithmetic, equality and arrays. The type `{v: Int | v > 0}` is a subtype of `Int`, and one refinement is a subtype of another when its predicate implies the other predicate. The checker collects the facts that hold at each point, from conditions, assignments and the refinements of the inputs, and asks an SMT solver, such as Z3, whether they imply what each operation needs. If the solver finds a counterexample, the program is rejected, as in the example. LiquidHaskell adds refinement types to Haskell and says that it "makes illegal values be unrepresentable".[^3] Liquid types infer most refinements from a small set of templates, so the programmer annotates only the important functions.

**Weaker forms in mainstream languages.** Most languages get some of the benefit without a solver. Types indexed by constants put a number in a type when the number is known at compile time: Go array types such as `[3]float64`, C++ templates such as `std::array<T, N>`, and Rust's const generics, where "Const generic parameters allow items to be generic over constant values".[^4] C++20 can state predicates on such values; each concept "is a predicate, evaluated at compile time, and becomes a part of the interface of a template where it is used as a constraint".[^5] C99 goes the other way: the type of a variable-length array depends on a run-time value, and "on another pass over the same code, it may be allocated with a different size", but C does not check that the sizes agree.[^6]

**Refinements checked at run time.** Where the compiler cannot prove a predicate, a type can still carry it. A smart constructor, a function that is the only way to make a value of the type, checks the predicate once, and all later code can trust it. Rust's `NonZero` is such a type in the standard library: "A value that is known not to equal zero."[^7] SQL databases attach predicates to types with domains: "A domain is essentially a data type with optional constraints (restrictions on the allowed set of values)."[^8] Python's `Annotated` is a "Special typing form to add context-specific metadata to an annotation", which libraries use for constraints such as minimum and maximum values.[^9]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `Annotated` metadata and checks at run time; Pydantic |
| JavaScript | idiom | smart constructors with private fields |
| TypeScript | idiom | branded types, literal types and template literal types |
| Java | idiom | records with compact constructors |
| Kotlin | idiom | value classes with `init` checks |
| C# | idiom | record structs with private constructors and factories |
| Go | idiom | unexported fields and constructors; array lengths in types |
| Rust | idiom | const generics, `NonZero` and validated newtypes |
| C | idiom | variably modified types, not checked; checked constructors |
| C++ | idiom | non-type template parameters and `requires` clauses |
| Bash | n/a | values are strings |
| PowerShell | native | validation attributes on variables, checked at each assignment |
| SQL | native | `CREATE DOMAIN` and `CHECK` constraints |
| Regex | n/a | no types |
| HTML | n/a | no types; form attributes such as `min` and `pattern` |
| CSS | n/a | no user-defined types |

No core general-purpose language has full dependent types or refinement types that the compiler proves with a solver, so each one gets the idiom verdict. The languages that have them, such as Idris, Agda, Lean, Rocq, F*, ATS and LiquidHaskell, are research languages and proof assistants. PowerShell and SQL are native in a narrower sense: they attach a predicate to a variable or a column type, and they check it each time a value is stored.

The example files show the two directions. Rust, C++ and Go put constant values in types, and the Rust and C++ fails blocks show the compiler rejecting a call because two lengths differ or a size predicate is false. C shows a type that depends on a run-time value but is not checked. Python, JavaScript, Java and C# carry a predicate in a type and check it once at run time, in a constructor or a decorator, and the C# file shows how a struct default can bypass the check.

## Variations and related ideas

- **Full dependent types.** Agda, Rocq, Lean and Idris let types contain any total expression. Agda is "an extension of Martin-Löf’s type theory".[^2]
- **Indexed types.** Haskell's generalized algebraic data types, `DataKinds` and type families, and OCaml's GADTs, put values such as lengths at the type level without full dependent types. See [sum types](/sum-types/).
- **Refinement type systems.** LiquidHaskell, F*, Flux for Rust and Liquid Java check predicates with an SMT solver. Dafny and SPARK Ada verify contracts in a similar way.
- **Subrange types.** Pascal's `0..100` and Ada's `range 0 .. 100` are refinements of integers, checked at run time and sometimes removed by the compiler.
- **Smart constructors and newtypes.** A wrapper type that only a checked function can make, often summed up as parse, don't validate. See [assertions](/assertions/) for checks that do not change the type.
- **Linear and quantitative types.** Idris 2 marks some arguments as used zero times, so lengths and proofs that exist only for the type checker are erased before the program runs.
- **Gradual verification.** Some systems check a refinement statically where they can and insert a run-time check elsewhere, as [gradual typing](/gradual-types/) does for ordinary types.

## Pitfalls

- **The proof burden.** A dependent type checker knows that `0 + m` is `m` from the definition of `+`, but not that `m + 0` is `m`. Code that reverses a `Vect` with an accumulator needs a proof of such facts, and the work can exceed the work to write the code.
- **Solver limits.** SMT solvers decide linear arithmetic, but nonlinear arithmetic, such as `x * y > 0`, and quantifiers can time out or fail. Refinement checkers then reject correct code or need hints.
- **Mathematical and machine integers.** A refinement proved over unbounded integers can fail with 32-bit or 64-bit values that overflow. Use a logic of bit vectors, or check the bounds of the machine type too.
- **Defaults and zero values bypass the constructor.** In Go any package can write `Port{}`, a port of 0, and in C# `default(Percent)` makes a value without the factory. Deserialization and reflection can also skip the check. Allow the zero value in the invariant, or make the type a class or an interface.
- **Metadata that nothing checks.** `Annotated` constraints and doc comments look like types, but a type checker ignores them, and a check happens only where code runs it.
- **Long compile times.** Type-level computation with C++ templates or Rust const generics creates one copy of the code for each value, and complex constraints give long error messages.

## Cost

Refinements and dependent types that the compiler proves cost nothing at run time, and they can make code faster: a proved bounds check can be removed, which was the aim of Dependent ML. Indexes and proofs are usually erased before the program runs; if they are not, a length stored as a unary natural number costs memory and time. A smart constructor costs one check when the value is made and none after. A refinement can also save space: `Option<NonZero<u32>>` fits in 4 bytes because zero is free to mean `None`. The main cost is at compile time, where type checking runs code and calls a solver, and in the programmer's time to write types, invariants and proofs.

## History

N. G. de Bruijn's Automath, from 1967, was the first language to check proofs with dependent types. Per Martin-Löf presented his intuitionistic type theory in 1972 and revised it through the 1970s and 1980s; it is the basis of Agda and of much later work.[^2] Thierry Coquand and Gérard Huet defined the calculus of constructions in 1985 and 1988, the basis of the Coq proof assistant, now named Rocq. On the refinement side, Pascal had subrange types in 1970 and Ada had range constraints in 1983. Tim Freeman and Frank Pfenning introduced refinement types for ML in 1991, and Hongwei Xi and Frank Pfenning used dependent types to remove array bounds checks in Dependent ML in 1998.

Lennart Augustsson's Cayenne, from 1998, and Conor McBride and James McKinna's Epigram, from 2004, applied dependent types to general programming. Ulf Norell's Agda 2 appeared in 2007, Edwin Brady described Idris in 2013 and released Idris 2 in 2020,[^1] and Leonardo de Moura started Lean in 2013. Patrick Rondon, Ming Kawaguchi and Ranjit Jhala introduced liquid types in 2008, which led to LiquidHaskell in 2014.[^3] Mainstream languages added weaker forms: C99 variable-length arrays in 1999,[^6] C++20 concepts in 2020,[^5] and Rust const generics in 2021.[^4]

## References
