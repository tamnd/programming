---
tag: EAVZR1
kind: concept
slug: hindley-milner
name: Type inference
aliases: [Hindley-Milner type inference, Damas-Milner, algorithm W, let-polymorphism, local type inference]
area: types/type-inference
level: core
summary: >-
  Type inference lets the compiler find static types without annotations. Local inference reads one initializer. Hindley-Milner inference solves type equations with unification.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1978
  people: [Roger Hindley, Robin Milner]
  note: Milner gave algorithm W, which infers a polymorphic type for each ML expression with Robinson's unification. Hindley had used unification in the same way for combinatory logic in 1969.
see_also: [static-and-dynamic-typing, parametric-polymorphism, subtyping, closures, higher-order-functions, type-conversion]
same_as:
  wikidata: Q586459
  wikipedia: Type_inference
sources:
  - paper: doi:10.2307/1995158
    title: J. Roger Hindley, The Principal Type-Scheme of an Object in Combinatory Logic, Transactions of the American Mathematical Society 146, 1969
    locator: the principal type-scheme of an object, found with the unification algorithm (as summarized in Milner 1978, section 1)
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978
    locator: 1 Introduction (ML had used the discipline for nearly 2 years; Hindley was the first to notice that Robinson's unification algorithm fits the problem)
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978
    locator: 2 Illustrations of the type discipline, Example 1 (map) and Example 2 (tagpair, generic type variables)
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978
    locator: 4.1 The Algorithm W (Proposition 5, the unification theorem of Robinson; W is probably complete)
  - paper: doi:10.1016/0022-0000(78)90014-4
    title: Robin Milner, A theory of type polymorphism in programming, Journal of Computer and System Sciences 17(3), 1978
    locator: the abstract (Semantic Soundness Theorem, well-typed programs cannot go wrong; W implemented for ML in Edinburgh LCF)
  - paper: doi:10.1145/582153.582176
    title: Luis Damas and Robin Milner, Principal type-schemes for functional programs, POPL 1982, pages 207 to 212
    locator: the sections Algorithm W and Completeness of W
  - paper: doi:10.1145/321250.321253
    title: J. A. Robinson, A Machine-Oriented Logic Based on the Resolution Principle, Journal of the ACM 12(1), 1965
    locator: the unification algorithm, which finds a most general unifier (as stated in Milner 1978, Proposition 5)
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.1 Overview of Types and Classes (a traditional Hindley-Milner polymorphic type system)
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html#x10-670004.1.4
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.1.4 Semantics of Types and Classes (the principal type of all expressions)
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html#x10-900004.5.2
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.5.2 Generalization
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html#x10-920004.5.4
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.5.4 Monomorphism (g is monomorphic in a; (g True, g 'c') is invalid)
  - url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html#x10-930004.5.5
    title: Haskell 2010 Language Report, chapter 4, Declarations and Bindings
    locator: 4.5.5 The Monomorphism Restriction, Rule 1
  - url: https://ocaml.org/manual/5.3/polymorphism.html#ss:valuerestriction
    title: The OCaml Manual, release 5.3, Polymorphism and its limitations
    locator: 6.1.2 The value restriction
  - url: https://ocaml.org/manual/5.3/polymorphism.html#ss:weak-types
    title: The OCaml Manual, release 5.3, Polymorphism and its limitations
    locator: 6.1.1 Weak polymorphism and mutation (let store = ref None)
  - url: https://openjdk.org/jeps/286
    title: "JEP 286: Local-Variable Type Inference"
    locator: Summary, Motivation (C++ auto, C# var, Go :=), Description and Scope Choices (an initializer avoids action at a distance errors); Release 10
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.4 Local Variable Declarations (the compile-time errors for var)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.4.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.4.1 Local Variable Declarators and Types (the type of the initializer; the null type is an error)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/declarations#implicitly-typed-local-variables
    title: C# language reference, Declaration statements
    locator: Implicitly typed local variables (implicitly typed local variables are strongly typed)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/statements#13622-implicitly-typed-local-variable-declarations
    title: C# language specification, Statements
    locator: 13.6.2.2 Implicitly typed local variable declarations (var z = null is an error)
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.type.auto.deduct
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.type.auto.deduct], paragraph 3 (auto is deduced with the rules for template argument deduction from a function call)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/temp.deduct.call
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[temp.deduct.call], paragraph 2 (if P is not a reference type, arrays and functions decay and top-level cv-qualifiers are ignored)"
  - url: https://go.dev/ref/spec#Short_variable_declarations
    title: The Go Programming Language Specification
    locator: Short variable declarations
  - url: https://go.dev/ref/spec#Constants
    title: The Go Programming Language Specification
    locator: Constants (the default types of untyped constants)
  - url: https://www.typescriptlang.org/docs/handbook/type-inference.html#best-common-type
    title: TypeScript Handbook, Type Inference
    locator: Best common type
  - url: https://www.typescriptlang.org/docs/handbook/type-inference.html#contextual-typing
    title: TypeScript Handbook, Type Inference
    locator: Contextual Typing
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#literal-types
    title: TypeScript Handbook, Everyday Types
    locator: Literal Types (let gives string, const gives the literal type)
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#return-type-annotations
    title: TypeScript Handbook, Everyday Types
    locator: Return Type Annotations
  - url: https://www.typescriptlang.org/docs/handbook/2/everyday-types.html#noimplicitany
    title: TypeScript Handbook, Everyday Types
    locator: noImplicitAny
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html#data-types
    title: The Rust Programming Language, 3.2 Data Types
    locator: Data Types (the compiler can usually infer the type; parse needs an annotation)
  - url: https://doc.rust-lang.org/book/ch03-03-how-functions-work.html#parameters
    title: The Rust Programming Language, 3.3 Functions
    locator: Parameters (a deliberate decision in Rust's design)
  - url: https://doc.rust-lang.org/book/ch08-01-vectors.html#creating-a-new-vector
    title: The Rust Programming Language, 8.1 Storing Lists of Values with Vectors
    locator: Creating a New Vector
  - url: https://kotlinlang.org/docs/basic-syntax.html#variables
    title: Kotlin documentation, Basic syntax
    locator: Variables
  - url: https://kotlinlang.org/docs/functions.html#return-types
    title: Kotlin documentation, Functions
    locator: Return types and Single-expression functions
  - url: https://mypy.readthedocs.io/en/stable/type_inference_and_annotations.html#type-inference
    title: mypy documentation, Type inference and type annotations
    locator: Type inference
  - url: https://mypy.readthedocs.io/en/stable/type_inference_and_annotations.html#explicit-types-for-collections
    title: mypy documentation, Type inference and type annotations
    locator: Explicit types for collections (Need type annotation for "l")
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf
    title: ISO/IEC 9899:2023 working draft N3096 (C23)
    locator: 6.7.9 Type inference
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_variables?view=powershell-7.5#types-of-variables
    title: PowerShell documentation, about_Variables
    locator: Types of variables
  - url: https://go.dev/ref/spec#Declarations_and_scope
    title: The Go Programming Language Specification
    locator: Declarations and scope (an identifier may be redeclared in an inner block)
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.type
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.type], paragraph 1 (an expression of type reference to T is adjusted to T)"
  - url: https://docs.python.org/3.14/library/typing.html
    title: The Python Standard Library, typing, Python 3.14
    locator: the note at the top of the page (the runtime does not enforce type annotations)
---

## Idea

A statically typed program gives each variable and each expression a type, and the compiler checks the types before the program runs. Type inference lets the compiler find many of these types itself. The programmer writes fewer annotations, and the check stays the same.

There are two main kinds. Local inference finds the type of one declaration from its initializer. Java `var`, C# `var`, C++ `auto` and Go `:=` work this way.[^15] Global inference finds the types of a whole group of definitions at once, functions included, from the way the code uses each value. ML and Haskell use the Hindley-Milner method, which solves equations between types with unification.[^2][^8]

In the Java program below, no local variable has a written type. The compiler gives `counts` the type `HashMap<String, Integer>`, `words` the type `String[]` and `total` the type `int`.[^17]

```java run id=idea
import java.util.HashMap;

public class Main {
    public static void main(String[] args) {
        var counts = new HashMap<String, Integer>();
        var words = "to be or not to be".split(" ");
        for (var word : words) {
            counts.merge(word, 1, Integer::sum);
        }
        var total = 0;
        for (var n : counts.values()) {
            total += n;
        }
        System.out.println(counts.get("to") + " " + total + " " + counts.getClass().getSimpleName());
    }
}
```

```output for=idea
2 6 HashMap
```

The type is fixed when the program is compiled. A later assignment of a value of another type does not compile, as it would with a written type. The C# reference says it directly: implicitly typed local variables are strongly typed.[^18] Type inference is a feature of static type systems. [[static-and-dynamic-typing]] explains the difference from a dynamically typed language, where a variable has no static type at all.

## How it works

### Local inference

Local inference looks at one declaration. The type of the variable is the type of its initializer, after a few adjustments that each language defines.

- **Java.** The type of a `var` variable is the type of the initializer, after an upward projection that removes capture variables. A `var` declaration with no initializer, with the `null` literal, or with a lambda expression is a compile-time error.[^16][^17] JEP 286 gives the reason for the required initializer: it avoids inference errors caused by code far from the declaration.[^15]
- **C#.** `var x;`, `var z = null;` and `var u = x => x + 1;` are errors. The reason for `null` is that `null` does not have a type.[^19]
- **C++.** The type of `auto` comes from the rules for template argument deduction in a function call. The compiler treats `auto x = e;` like a call `f(e)` of an invented `template<class U> void f(U u)`.[^20] When the parameter is not a reference, the deduction ignores the top-level `const` and turns an array into a pointer.[^21] The type of an expression is never a reference type, so plain `auto` makes a copy.[^39] `auto&` keeps the reference.
- **Go.** `x := e` is a short form of `var x = e`.[^22] An untyped constant gets its default type: `bool`, `rune`, `int`, `float64`, `complex128` or `string`.[^23]
- **TypeScript.** A variable gets the type of its initializer, and a function gets the return type of its `return` statements.[^27] A `let` that holds `"Hello World"` gets the type `string`. A `const` gets the literal type `"Hello World"`, because its value cannot change.[^26] For an array of mixed elements, the compiler picks a common type of the elements, so `[0, 1, null]` has the type `(number | null)[]`.[^24] Inference also works from the outside in. In `window.onmousedown = function (mouseEvent) {...}`, the location of the function gives `mouseEvent` its type. The handbook calls this contextual typing.[^25]
- **Kotlin.** `val` and `var` take the type of their initializer. A function with an expression body gets its return type by inference, but a function with a block body does not.[^32][^33]
- **C.** C23 adds the storage-class specifier `auto` for inference. The type is the type of the initializer after lvalue, array to pointer and function to pointer conversion.[^36]

### Global inference with unification

Hindley-Milner inference does not stop at one declaration. It gives every variable and every parameter a type, also when no initializer exists. The method has four steps, and the worked example below numbers them.

- **Type variables.** Each unknown type gets a fresh type variable, such as `t1`.
- **Constraints.** Each use of a value adds an equation. A call `f x` says that the type of `f` is `type of x -> t2` for a fresh `t2`, the type of the result.
- **Unification.** Robinson's unification algorithm solves the equations. Its result is a substitution: a map from type variables to types. Unification finds the most general substitution, so the result has no choice in it that the program did not force.[^4][^7]
- **Generalization.** At a `let`, each type variable that is not in the types of the enclosing variables becomes generic. Each later use of the name gets a fresh copy of the generic variables.[^3][^10]

Hindley used unification in this way for combinatory logic in 1969. Milner found the method again for ML and called his algorithm W.[^1][^2] The worked example below infers the type of `twice`, a function that applies `f` two times.

```text
expression   fn f => fn x => f (f x)

step 1   f : t1          x : t2
step 2   f x             t1 = t2 -> t3
         f (f x)         t1 = t3 -> t4
step 3   unify  t2 -> t3  with  t3 -> t4
           t2 = t3,  t3 = t4
         substitution    t3 := t2,  t4 := t2,  t1 := t2 -> t2
         whole type      t1 -> t2 -> t4  becomes  (t2 -> t2) -> t2 -> t2
step 4   no type variable occurs in the environment, so all are generic
         twice : for all a. (a -> a) -> a -> a
```

**The occurs check.** Unification must not bind a type variable to a type that contains the same variable. In `fn x => x x`, the parameter `x` has the type `t1`. The call `x x` asks for `t1 = t1 -> t2`. No finite type solves this equation, so inference fails.

**Let-polymorphism and lambda-bound monomorphism.** Only a name bound by `let` becomes generic. A parameter of a function stays monomorphic: each use inside the body has the same type. In Milner's `map` example, the two uses of `map` get different instances, but the parameter `f` must have the same type at each use.[^3] The Haskell report shows the same rule. Inside `f x = let g y z = ([x,y], z) in ...`, the type of `g` is generic in `b` but not in `a`, because `a` is the type of `x` in the environment. So `(g True, g 'c')` is invalid.[^11]

The Python program below is a small version of algorithm W. Expressions are tuples. The program keeps one substitution for the whole run and extends it at each step of unification. It infers four types, accepts `id` at two types under `let`, rejects the same code with `id` as a parameter, and stops at the occurs check.

```python run id=algorithm-w
import itertools


class TVar:
    def __init__(self, id):
        self.id = id


class TCon:
    def __init__(self, name, *args):
        self.name = name
        self.args = args


INT = TCon("int")
BOOL = TCon("bool")


def arrow(a, b):
    return TCon("->", a, b)


class Scheme:
    def __init__(self, generic, type):
        self.generic = generic  # ids of the generic type variables
        self.type = type


class InferError(Exception):
    pass


def show(t, names):
    if isinstance(t, TVar):
        return names.setdefault(t.id, chr(ord("a") + len(names)))
    if not t.args:
        return t.name
    left, right = (show(a, names) for a in t.args)
    if t.args[0].__class__ is TCon and t.args[0].args:
        left = f"({left})"
    return f"{left} {t.name} {right}"


class Inferer:
    def __init__(self):
        self.subst = {}  # type variable id -> type
        self.ids = itertools.count()

    def fresh(self):
        return TVar(next(self.ids))

    def resolve(self, t):
        if isinstance(t, TVar):
            if t.id in self.subst:
                return self.resolve(self.subst[t.id])
            return t
        return TCon(t.name, *(self.resolve(a) for a in t.args))

    def occurs(self, id, t):
        if isinstance(t, TVar):
            return t.id == id
        return any(self.occurs(id, a) for a in t.args)

    def unify(self, a, b):
        a, b = self.resolve(a), self.resolve(b)
        names = {}
        if isinstance(a, TVar):
            if isinstance(b, TVar) and a.id == b.id:
                return
            if self.occurs(a.id, b):
                raise InferError(f"occurs check: {show(a, names)} occurs in {show(b, names)}")
            self.subst[a.id] = b
        elif isinstance(b, TVar):
            self.unify(b, a)
        elif a.name != b.name or len(a.args) != len(b.args):
            raise InferError(f"cannot unify {show(a, names)} with {show(b, names)}")
        else:
            for x, y in zip(a.args, b.args):
                self.unify(x, y)

    def free(self, t):
        t = self.resolve(t)
        if isinstance(t, TVar):
            return {t.id}
        return set().union(*(self.free(a) for a in t.args))

    def generalize(self, env, t):
        in_env = set()
        for s in env.values():
            in_env |= self.free(s.type) - s.generic
        return Scheme(self.free(t) - in_env, self.resolve(t))

    def instantiate(self, s):
        fresh = {id: self.fresh() for id in s.generic}

        def copy(t):
            t = self.resolve(t)
            if isinstance(t, TVar):
                return fresh.get(t.id, t)
            return TCon(t.name, *(copy(a) for a in t.args))

        return copy(s.type)

    def infer(self, env, e):
        kind = e[0]
        if kind == "int":
            return INT
        if kind == "bool":
            return BOOL
        if kind == "var":
            return self.instantiate(env[e[1]])
        if kind == "fn":
            _, x, body = e
            tx = self.fresh()  # a parameter is never generic
            return arrow(tx, self.infer({**env, x: Scheme(set(), tx)}, body))
        if kind == "app":
            _, f, a = e
            tf = self.infer(env, f)
            ta = self.infer(env, a)
            result = self.fresh()
            self.unify(tf, arrow(ta, result))
            return result
        _, x, bound, body = e  # let
        t = self.infer(env, bound)
        return self.infer({**env, x: self.generalize(env, t)}, body)


def src(e):
    kind = e[0]
    if kind == "int":
        return str(e[1])
    if kind == "bool":
        return "true" if e[1] else "false"
    if kind == "var":
        return e[1]
    if kind == "fn":
        return f"fn {e[1]} => {src(e[2])}"
    if kind == "app":
        f, a = src(e[1]), src(e[2])
        if e[1][0] in ("fn", "let"):
            f = f"({f})"
        if e[2][0] in ("fn", "let", "app"):
            a = f"({a})"
        return f"{f} {a}"
    return f"let {e[1]} = {src(e[2])} in {src(e[3])}"


def var(name):
    return ("var", name)


def app(f, *args):
    for a in args:
        f = ("app", f, a)
    return f


ENV = {
    "pair": Scheme({-1, -2}, arrow(TVar(-1), arrow(TVar(-2), TCon("*", TVar(-1), TVar(-2))))),
    "add": Scheme(set(), arrow(INT, arrow(INT, INT))),
}

identity = ("fn", "x", var("x"))
use_twice = app(var("pair"), app(var("id"), ("int", 1)), app(var("id"), ("bool", True)))
examples = [
    identity,
    ("fn", "x", app(var("add"), var("x"), ("int", 1))),
    ("fn", "f", ("fn", "x", app(var("f"), app(var("f"), var("x"))))),
    ("fn", "f", ("fn", "g", ("fn", "x", app(var("g"), app(var("f"), var("x")))))),
    ("let", "id", identity, use_twice),
    ("fn", "id", use_twice),
    ("fn", "x", app(var("x"), var("x"))),
]

for e in examples:
    inferer = Inferer()
    try:
        t = inferer.infer(ENV, e)
        print(f"{src(e)}\n  : {show(inferer.resolve(t), {})}")
    except InferError as err:
        print(f"{src(e)}\n  error: {err}")
```

```output for=algorithm-w
fn x => x
  : a -> a
fn x => add x 1
  : int -> int
fn f => fn x => f (f x)
  : (a -> a) -> a -> a
fn f => fn g => fn x => g (f x)
  : (a -> b) -> (b -> c) -> a -> c
let id = fn x => x in pair (id 1) (id true)
  : int * bool
fn id => pair (id 1) (id true)
  error: cannot unify int with bool
fn x => x x
  error: occurs check: a occurs in a -> b
```

The third result is the type of the worked example. Under `let`, `id` has the scheme `for all a. a -> a`, so the two uses get the types `int -> int` and `bool -> bool`. As a parameter, `id` has one type variable, and the second use forces `int` to equal `bool`.

### Inference inside a function body

Rust takes a middle way. The compiler infers the types of the local variables in a function body, also from later uses. But each function signature must give the types of its parameters and its result. The Rust Book calls this a deliberate decision: the annotations in signatures mean that the compiler almost never needs them elsewhere.[^29][^30] An empty vector shows inference from a later use. `let mut v = Vec::new();` followed by `v.push(5);` needs no annotation, because the push decides the element type.[^31]

## In languages

The example files show inference in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | library | CPython infers nothing; a type checker such as mypy infers a type from the first value[^34] |
| JavaScript | n/a | no static types |
| TypeScript | native | initializers, return statements, contextual typing, literal widening |
| Java | native | `var` for local variables with an initializer[^16] |
| C | native | C23 `auto` in an object definition with an initializer[^36] |
| C++ | native | `auto` and `decltype(auto)`, with the rules of template argument deduction[^20] |
| C# | native | `var` for local variables with an initializer[^19] |
| Go | native | `:=` and `var` with no type; untyped constants get default types[^22][^23] |
| Rust | native | inference inside a function body; signatures need types[^30] |
| Kotlin | native | `val` and `var` with an initializer; the return type of an expression body[^32][^33] |
| PowerShell | n/a | variables are loosely typed and hold a value of any .NET type[^37] |
| Bash | n/a | no static types |
| SQL | n/a | a query declares no variables; column types come from the table definitions |
| Regex | n/a | a pattern has no types |
| HTML/CSS | n/a | markup and style rules have no types |

The Python runtime does not enforce type annotations.[^40] A separate type checker reads them. mypy infers the type of a variable from the value that is first assigned to it. It cannot always infer the type of an empty list or dictionary, and it then asks for an annotation, as in `Need type annotation for "l"`.[^34][^35]

## Variations and related ideas

- **Local and global inference.** Local inference needs an initializer and looks only at one declaration.[^15] Hindley-Milner inference also finds the types of function parameters from the uses in the body.[^3] Rust infers inside a body and requires types at the boundary of each function.[^30]
- **Principal types.** Take an expression that has a type. Hindley-Milner inference finds one type for it, and each other type of the expression is an instance of that type. Damas and Milner proved that algorithm W finds it.[^6] The Haskell report states that its type system can infer the principal type of all expressions.[^9]
- **Contextual typing.** TypeScript also sends type information from the outside in, from the expected type to a function expression.[^25]
- **The value restriction.** In ML, a reference cell must not become polymorphic. Otherwise code could store a value of one type and read it as another. OCaml does not generalize the type of a mutable value or of the result of a function call. The type of `ref None` gets a weak type variable, written `'_weak1`, that the first use fixes.[^14][^13] The call `(fun x -> x) (fun x -> x)` also has a weak type. Eta expansion is the fix.[^13]
- **The monomorphism restriction.** Take a binding of one variable with no arguments, such as `n = e`. Haskell does not generalize a constrained type variable of such a binding unless the binding has a type signature.[^12]
- **Type classes.** Haskell extends Hindley-Milner inference with class constraints, such as `Eq a`, in the inferred type.[^8] [[parametric-polymorphism]] explains generic types and constraints.
- **Subtyping.** Hindley-Milner inference solves equations between types. With [[subtyping]], one value can have many types. For an array literal, TypeScript picks a common type of the elements, and that type can be a union.[^24]

## Pitfalls

### `var` needs an initializer with a type

Java infers the type of `var` only from the initializer. The `null` literal has the null type, and the JLS makes it an error for `var`.[^17] C# rejects `var z = null;` for the same reason.[^19]

```java fails expect=compile id=java-null
public class Main {
    public static void main(String[] args) {
        var name = null;
        name = "Ada";
        System.out.println(name);
    }
}
```

`javac` stops with `cannot infer type for local variable name` and the reason `variable initializer is 'null'`. The fix is a written type, `String name = null;`, or a real initial value.

### Rust cannot choose among many types

`parse` can return many types. When nothing in the function body fixes the type, the compiler stops and asks for an annotation.[^29]

```rust fails expect=compile id=rust-annotations
fn main() {
    let guess = "42".parse().expect("Not a number!");
    println!("{}", guess);
}
```

`rustc` stops with error E0284, `type annotations needed`. The fix is an annotation on the variable, `let guess: u32`, or a turbofish on the call, `"42".parse::<u32>()`.

```rust run id=rust-fix
fn main() {
    let guess: u32 = "42".parse().expect("Not a number!");
    let other = "7".parse::<u8>().expect("Not a number!");
    println!("{} {}", guess + 1, other);
}
```

```output for=rust-fix
43 7
```

### C++ `auto` drops the reference

`auto` deduces a type as a template parameter `U` by value. The reference and the top-level `const` of the initializer are gone, so the variable is a copy.[^20][^21] Here `account` is a copy of the element, and the update does not reach the vector.

```cpp wrong id=cpp-copy
#include <print>
#include <string>
#include <vector>

struct Account {
    std::string owner;
    int balance;
};

Account& find(std::vector<Account>& accounts, const std::string& owner) {
    for (auto& a : accounts) {
        if (a.owner == owner) {
            return a;
        }
    }
    return accounts.front();
}

int main() {
    std::vector<Account> accounts{{"ada", 100}, {"grace", 50}};
    auto account = find(accounts, "grace");
    account.balance += 25;
    std::println("{}", accounts[1].balance);
}
```

```output for=cpp-copy
50
```

The program prints the old balance. The fix is `auto& account = find(accounts, "grace");`. `decltype(auto)` also keeps the reference, because it deduces the type that `decltype` gives for the initializer.[^20]

```cpp run id=cpp-ref
#include <print>
#include <string>
#include <vector>

struct Account {
    std::string owner;
    int balance;
};

Account& find(std::vector<Account>& accounts, const std::string& owner) {
    for (auto& a : accounts) {
        if (a.owner == owner) {
            return a;
        }
    }
    return accounts.front();
}

int main() {
    std::vector<Account> accounts{{"ada", 100}, {"grace", 50}};
    auto& account = find(accounts, "grace");
    account.balance += 25;
    std::println("{}", accounts[1].balance);
}
```

```output for=cpp-ref
75
```

### Go `:=` in an inner block declares a second variable

`:=` may redeclare a variable only when the variable was declared earlier in the same block.[^22] In an inner block, `:=` declares another variable, and that variable hides the outer one until the block ends.[^38] Here `limit, err :=` inside the `if` makes a second `limit`.

```go wrong id=go-shadow
package main

import (
	"fmt"
	"strconv"
)

func main() {
	limit := 10
	input := "25"
	if input != "" {
		limit, err := strconv.Atoi(input)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println("inside:", limit)
	}
	fmt.Println("limit:", limit)
}
```

```output for=go-shadow
inside: 25
limit: 10
```

The outer `limit` keeps the value 10. The fix is to declare `err` before the block and to assign with `=`.

```go run id=go-assign
package main

import (
	"fmt"
	"strconv"
)

func main() {
	limit := 10
	input := "25"
	if input != "" {
		var err error
		limit, err = strconv.Atoi(input)
		if err != nil {
			fmt.Println(err)
			return
		}
	}
	fmt.Println("limit:", limit)
}
```

```output for=go-assign
limit: 25
```

### TypeScript falls back to `any`

When TypeScript has no annotation and no context for a type, it usually uses `any`, and `any` is not type-checked.[^28] A parameter with no annotation is such a case. Here both parameters of `addPoints` are `any`, so the compiler accepts a string, and `+` joins two strings.

```typescript wrong id=ts-any
function addPoints(score, bonus) {
  return score + bonus;
}

const fields = "40,2".split(",");
console.log(addPoints(fields[0], 2));
```

```output for=ts-any
402
```

The fix is a type on each parameter, a conversion at the call, and the compiler option `noImplicitAny`, which makes each implicit `any` an error.[^28]

```typescript run id=ts-typed
function addPoints(score: number, bonus: number): number {
  return score + bonus;
}

const fields = "40,2".split(",");
console.log(addPoints(Number(fields[0]), 2));
```

```output for=ts-typed
42
```

## Cost

- **Run time.** Inference happens when the program is compiled. An inferred variable has a static type, as a variable with a written type has.[^17][^18]
- **Error messages.** A type error found by unification can show far from its cause. A constraint from one line can fix a type that another line uses. JEP 286 requires an initializer for `var` to avoid such errors.[^15] The Rust Book adds that the compiler gives more helpful error messages when it knows the types that a function expects.[^30]
- **Readability.** The type of an inferred variable is not on the page. The TypeScript handbook notes that some code bases write return types for documentation and to prevent accidental changes.[^27]
- **Expressiveness.** Hindley-Milner inference needs no annotations, but it gives a parameter only one type in the body.[^3] A mutable cell or a function result is not generalized under the value restriction.[^13]

## History

- **1965.** J. A. Robinson published the unification algorithm as a part of resolution in logic.[^7][^4]
- **1969.** J. Roger Hindley showed that an object of combinatory logic has a principal type scheme, found with unification.[^1][^2]
- **1978.** Robin Milner published algorithm W and the type discipline of ML. ML had used the discipline for nearly two years. The paper proves that well-typed programs cannot go wrong.[^2][^5]
- **1982.** Luis Damas and Robin Milner proved that algorithm W finds the principal type scheme.[^6]
- **2010.** The Haskell 2010 report describes its type system as a traditional Hindley-Milner polymorphic type system.[^8]
- **2023.** The C23 working draft N3096, of April 2023, includes type inference with `auto`.[^36]

## References
