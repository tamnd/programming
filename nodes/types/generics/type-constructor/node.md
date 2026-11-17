---
tag: SSJ9MR
kind: concept
slug: type-constructor
name: Type constructor
aliases: [type operator, generic type, parameterized type, generic type definition, unbound generic type, kind, higher-kinded type]
area: types/generics
level: core
summary: >-
  A type constructor builds a type from other types: List makes List of Int from Int. Its arity is the number of types it takes, and its kind describes what it takes and gives.
applies_to: core-languages
viz: optional
formal: none
see_also: [parametric-polymorphism, composite-type, function-type, algebraic-data-type, recursive-type, type-erasure, list, map-and-dictionary]
same_as:
  wikidata: Q7860899
  wikipedia: Type_constructor
sources:
- url: https://en.wikipedia.org/wiki/Type_constructor
  title: Type constructor, Wikipedia
  locator: Whole article
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: Built-in Types, Python 3.14 documentation
  locator: Generic Alias Type
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch4.html
  title: Haskell 2010 Language Report, Chapter 4, Declarations and Bindings
  locator: 4.1.1 Kinds; 4.1.2 Syntax of Types
- url: https://learn.microsoft.com/en-us/dotnet/standard/generics/
  title: Generics in .NET, .NET documentation
  locator: Defining and using generics; Generics terminology
- url: https://docs.oracle.com/javase/tutorial/java/generics/types.html
  title: Generic Types, The Java Tutorials
  locator: Whole page
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Instantiations
- url: https://en.cppreference.com/w/c/language/type.html
  title: Type, C reference, cppreference.com
  locator: Type classification
- url: https://en.wikipedia.org/wiki/Kind_(type_theory)
  title: Kind (type theory), Wikipedia
  locator: Introduction; Examples; Kinds in Haskell
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/types
  title: Types, C# language specification
  locator: 8.4 Constructed types; 8.4.4 Bound and unbound types; 8.4.5 Open and closed types
- url: https://en.cppreference.com/w/cpp/language/template_parameters.html
  title: Template parameters and template arguments, C++ reference, cppreference.com
  locator: Template template parameter
- url: https://docs.oracle.com/javase/tutorial/java/generics/rawTypes.html
  title: Raw Types, The Java Tutorials
  locator: Whole page
- url: https://doc.rust-lang.org/book/ch10-01-syntax.html
  title: Generic Data Types, The Rust Programming Language
  locator: Performance of Code Using Generics
---

## Idea

`int` and `str` are types: a value can have the type `int`. `list` is different. A list holds items of some type, and `list[int]` and `list[str]` are two types made from one name. `list` takes a type and gives a type, so it works like a function from types to types. That function is a type constructor. `dict` takes two types, a key type and a value type. Python shows them at run time.

```python run id=idea
from typing import get_args, get_origin

# Built-in type constructors: list takes one type, dict takes two.
print(list[int], dict[str, list[int]])
print(get_origin(dict[str, int]), get_args(dict[str, int]))


# A generic class is a type constructor that the program defines.
class Box[T]:
    def __init__(self, item: T) -> None:
        self.item = item


# A generic type alias is one too.
type Pair[A, B] = tuple[A, B]

print(Box[str], Pair[int, str], Pair.__type_params__)

# Python does not check the argument when the program runs.
b = Box[int]("not an int")
print(type(b).__name__, b.item)
```

```output for=idea
list[int] dict[str, list[int]]
<class 'dict'> (<class 'str'>, <class 'int'>)
__main__.Box[str] Pair[int, str] (A, B)
Box not an int
```

`list[int]` is a type made by applying `list` to `int`, and `dict[str, list[int]]` applies `dict` to two types, one of which is itself made by `list`. `get_origin` returns the constructor and `get_args` returns the types it was applied to. `Box` and `Pair` are type constructors that the program defines. The last line shows that the type arguments are for a static checker: Python makes a plain `Box` and does not look at `int`.

## How it works

**Types from types.** A type constructor "is a feature of a typed formal language that builds new types from old ones"[^1]. "Abstractly, a type constructor is an n-ary type operator taking as argument zero or more types, and returning another type"[^1]. In Python, `list[int]` "is a GenericAlias object created by subscripting the list class with the argument int"[^2]. In Haskell, "Maybe and IO are unary type constructors"[^3].

**Arity.** The arity of a type constructor is the number of types it takes. "Basic types are considered to be built using nullary type constructors"[^1], so `Int` has arity 0. A list or an option type takes one type. A map takes two: the .NET type `Dictionary<TKey,TValue>` "has two type parameters, TKey and TValue"[^4]. A function type takes the parameter types and the result type. "Some type constructors take another type as an argument, e.g., the constructors for product types, function types, power types and list types"[^1].

**Declared constructors.** In most languages with generics, a generic class, interface or struct is a type constructor. "A generic type is a generic class or interface that is parameterized over types"[^5]. Applying it gives a type: "An invocation of a generic type is generally known as a parameterized type"[^5], such as `Box<Integer>`. In Go, a generic type "is instantiated by substituting type arguments for the type parameters"[^6]. A recursive type can use the constructor in its own definition, as a tree uses a list of trees.

**Built-in constructors.** Some type constructors have special syntax. In Haskell, "The list type is written as [] and has kind ∗→∗"[^3], and the function type `(->)` takes two types. C has no generics, but its array, pointer and function types are derived types[^7]: `int[10]`, `int *` and `int (int)` are built from `int`.

**Kinds.** Types classify values, and kinds classify types. A kind "is the type of a type constructor"[^8]. In Haskell, "The symbol ∗ represents the kind of all nullary type constructors"[^3], and "κ1 → κ2 is the kind of types that take a type of kind κ1 and return a type of kind κ2"[^3]. So `Int` has kind ∗, a list has kind ∗→∗, and a pair has kind ∗→∗→∗. A kind checks that a type expression is valid: a list applied to `Int` is a type, but a list applied to nothing, or applied to a list constructor, is not.

**A constructor is not a type.** A value has a type of kind ∗ only. The C# specification says: "An unbound generic type is not itself a type, and cannot be used as the type of a variable, argument or return value, or as a base type"[^9]. A variable can be a `List<int>`, but not a `List<>`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list[int]`, `dict[str, int]`; generic classes `class Box[T]`; generic type aliases `type Pair[A, B]` |
| JavaScript | n/a | no types in the language, so no type constructors |
| TypeScript | native | `Array<T>`, `Map<K, V>`; generic interfaces, classes and type aliases |
| Java | native | generic classes and interfaces; `List<String>`; raw types for old code |
| Kotlin | native | generic classes and interfaces; `List<String>`, `Map<K, V>` |
| C# | native | generic classes, structs and interfaces; `List<int>`; `typeof(List<>)` for the unbound type |
| Go | native | built-in slice, map, pointer and channel types; generic types with type parameters |
| Rust | native | generic structs and enums: `Vec<T>`, `Option<T>`, `HashMap<K, V>` |
| C | idiom | built-in array, pointer and function types only; macros make a struct for each element type |
| C++ | native | class templates such as `std::vector<T>`; template template parameters |
| Bash | n/a | no types |
| PowerShell | native | .NET generic types, written as `[System.Collections.Generic.List[int]]` |
| SQL | n/a | column types are fixed by the database, and a query defines no type constructors |
| Regex | n/a | a pattern has no types |
| HTML | n/a | HTML has no types |
| CSS | n/a | CSS has no types in this sense |

Most languages with generics let the program declare its own type constructors as generic classes or types. Go and C have built-in constructors with special syntax, such as `[]int` and `int *`. C++ goes further than the others: a template can take a class template as a parameter. Python and TypeScript check the type arguments only before the program runs.

The example files show these differences. Python shows `get_origin` and `get_args`. TypeScript shows a generic type alias and a generic interface. Java shows a generic class and a raw type. C# shows an unbound generic type and its arity. Go shows a generic type and the built-in map type. Rust shows `Option` and `Vec` with the type `Vec<Option<i32>>`. C shows a macro that makes a struct for each element type. C++ shows a template template parameter.

## Variations and related ideas

- **Higher-kinded types.** A type constructor can take another type constructor. "(∗→∗)→∗ is the kind of a higher-order type operator from unary type constructors to proper types"[^8]. Haskell allows this, and C++ has template template parameters, such as `template<typename> typename C` in the declaration of a class that uses `C<K>` and `C<V>`[^10]. Most languages with generics do not: "Since higher-order type operators are uncommon in programming languages, in most programming practice, kinds are used to distinguish between data types and the types of constructors which are used to implement parametric polymorphism"[^8].
- **Partial application.** In Haskell, a type constructor "takes one or more type arguments, and produces a data type when enough arguments are supplied, i.e. it supports partial application thanks to currying"[^8]. A pair constructor applied to one type gives a constructor of kind ∗→∗.
- **Data constructors.** A type constructor builds a type. A data constructor, such as `Just` or `Some`, builds a value of that type. In Haskell, both start with a capital letter: "As with data constructors, the names of type constructors start with uppercase letters"[^3]. See [algebraic data types](/algebraic-data-type/).
- **Type erasure.** Java removes the type arguments when it compiles, and Python does not keep them in the objects: "parameterized generics erase type parameters during object creation"[^2]. C# keeps them: "At run-time, all of the code within a generic type declaration is executed in the context of a closed constructed type that was created by applying type arguments to the generic declaration"[^9]. See [type erasure](/type-erasure/).

## Pitfalls

- **A raw type in Java.** "A raw type is the name of a generic class or interface without any type arguments"[^11]. Java accepts it for old code, but the compiler cannot check the items: "When using raw types, you essentially get pre-generics behavior"[^11]. Write `List<String>`, not `List`. The Java example shows a raw type.
- **Type arguments that nobody checks.** Python does not check the type arguments when the program runs. The documentation shows that `list[str]([1, 2, 3])` gives `[1, 2, 3]`[^2], and "The Python runtime does not enforce type annotations"[^2]. TypeScript erases the types too. Run a type checker to find these errors.
- **A missing type argument.** Most languages with generics reject a constructor in the place of a type. In Rust, a parameter of type `Vec` with no type argument is a compile error. The Rust example shows the error.
- **Two types that look the same.** `List<int>` and `List<long>` are different types, even when `int` converts to `long`. A function that takes one does not take the other. The rules for this are [variance](/subtyping/).

## Cost

A type constructor costs nothing when the program runs in a language that erases the type arguments, such as Python, TypeScript and Java. Rust makes separate code for each type argument instead: "Monomorphization is the process of turning generic code into specific code by filling in the concrete types that are used when compiled"[^12]. So the program does not get slower, but each `Vec<T>` with a different `T` adds machine code. In .NET, the code of a generic type runs "in the context of a closed constructed type"[^9].

## History

The Haskell report defines kinds and type application for Haskell type expressions: "If t1 is a type of kind κ1 → κ2 and t2 is a type of kind κ1, then t1 t2 is a type expression of kind κ2"[^3]. In Haskell, "all data type declarations are considered to declare type constructors"[^1]. Benjamin Pierce treats type operators and kinding in chapter 29 of *Types and Programming Languages* (2002)[^1].
