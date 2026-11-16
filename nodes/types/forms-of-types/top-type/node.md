---
tag: 2ES327
kind: concept
slug: top-type
name: Top type
aliases: [any type, universal type, universal supertype, Object, object, Any, 'Any?', unknown, 'interface{}', '⊤']
area: types/forms-of-types
level: advanced
summary: >-
  The top type is a supertype of every type, so a variable of it can hold any value. Java and C# call it Object, Python object, Kotlin Any? and TypeScript unknown.
applies_to: core-languages
viz: optional
formal: none
see_also: [bottom-type, subtyping, type, gradual-types, static-and-dynamic-typing, universal-object-methods, class-inheritance, runtime-reflection]
same_as:
  wikidata: Q4129836
  wikipedia: Any_type
sources:
- url: https://en.wikipedia.org/wiki/Any_type
  title: Any type, Wikipedia
  locator: Introduction; Support in programming languages; Other languages
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html
  title: Java SE 25 API, Class Object
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/reference-types
  title: Built-in reference types, C# language reference
  locator: The object type
- url: https://docs.python.org/3.14/library/functions.html
  title: Built-in Functions, Python 3.14 documentation
  locator: class object
- url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin/-any/
  title: Any, Kotlin Standard Library API reference
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Interface types; Type assertions
- url: https://en.cppreference.com/w/cpp/utility/any.html
  title: std::any, C++ reference, cppreference.com
  locator: Description
- url: https://www.typescriptlang.org/docs/handbook/2/functions.html
  title: More on Functions, The TypeScript Handbook
  locator: Other Types to Know About, unknown
- url: https://doc.rust-lang.org/std/any/index.html
  title: Module std::any, The Rust Standard Library
  locator: Any and TypeId
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: The Any type
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/types/boxing-and-unboxing
  title: Boxing and Unboxing, C# programming guide
  locator: Boxing
---

## Idea

Sometimes code must accept a value of any type: a list that holds numbers and strings, a function that logs whatever it gets, or a field whose type the program learns only when it runs. The top type is the type that every value has. Every other type is a subtype of it, so a value of any type fits in a variable of the top type. The price is that the code knows nothing about such a value, so it must test the real type before it can do more than pass the value on. Python and Java call the top type `object` and `Object`.

```python run id=idea
values: list[object] = [42, "hi", [1, 2], None, len, object]
print(all(isinstance(v, object) for v in values))
print([type(v).__name__ for v in values])
print(int.__mro__, bool.__mro__[-1])


def describe(x: object) -> str:
    # With the type object, code must test the type before it uses the value.
    if isinstance(x, str):
        return x.upper()
    if isinstance(x, int):
        return str(x + 1)
    return repr(x)


print(describe("hi"), describe(41), describe([1]))
print(issubclass(type, object), isinstance(object, object))
```

```output for=idea
True
['int', 'str', 'list', 'NoneType', 'builtin_function_or_method', 'type']
(<class 'int'>, <class 'object'>) <class 'object'>
HI 42 [1]
True True
```

Every value in the list is an instance of `object`: a number, a string, a list, `None`, a function and even a class. The method resolution order of every class ends with `object`. `describe` takes an `object`, so a type checker allows only the operations that every value has, and the function tests the type with `isinstance` before it calls `upper` or adds 1.

## How it works

**A supertype of every type.** "In type theory and computer science, type systems include a top, universal, or any type (often represented with the down tack (⊤) symbol), which includes all other types as subtypes"[^1]. "In object-oriented languages, it is often called Object, as it represents all possible objects"[^1]. It is the dual of the [bottom type](/bottom-type/), which is a subtype of every type and has no values.

**Two ideas with one name.** "In statically-typed languages, there are two different, often confused, concepts when discussing the top type"[^1]. One is a universal base class, the root of the class hierarchy, which objects have when the program runs. The other is a static type to which code can assign any value, such as `void*` in C or `interface{}` in Go, that is not the class of any object[^1].

**A universal base class.** In Java, "Class Object is the root of the class hierarchy. Every class has Object as a superclass"[^2]. In C#, the `object` "type is an alias for System.Object in .NET. In the unified type system of C#, all types, predefined and user-defined, reference types and value types, inherit directly or indirectly from System.Object"[^3]. In Python, `object` "is the ultimate base class of all other classes"[^4]. In Kotlin, `Any` is "The root of the Kotlin class hierarchy. Every Kotlin class has Any as a superclass"[^5], but a variable of type `Any` cannot hold `null`, so "the top type is kotlin.Any?, a nullable form of kotlin.Any"[^1].

**A static type for any value.** In Go, "all types implement the empty interface", and "the predeclared type any is an alias for the empty interface"[^6]. In C and C++, "the void pointer type (void*) can accept any non-function pointer"[^1], but it does not record the type of the data it points to. C++ and Rust have no root class: "std::any (in C++) and std::any::Any (in Rust) are not actually top types in the type-system, but rather type-erased containers, from which the data is obtained through downcasting"[^1]. The C++ class `any` "describes a type-safe container for single values of any copy constructible type"[^7].

**Using a value of the top type.** The top type has almost no operations, so code must test the type and then cast down. "Languages with runtime typing often provide downcasting to allow discovering a more specific type for an object at runtime"[^1]. In TypeScript, "The unknown type represents any value"[^8], and the compiler allows an operation on it only after code narrows the type with a test such as `typeof`. A Rust `&dyn Any` "has the is and downcast_ref methods, to test if the contained value is of a given type, and to get a reference to the inner value as a type"[^9].

**Top versus any.** Some languages have a second type that also accepts every value but turns off the checks. In Python, "A static type checker will treat every type as assignable to Any and Any as assignable to every type"[^10], so `Any` is not a top type: it is the dynamic type of [gradual typing](/gradual-types/). The Python documentation puts the difference this way: "Use object to indicate that a value could be any type in a typesafe manner. Use Any to indicate that a value is dynamically typed"[^10]. TypeScript has the same pair, `unknown` and `any`[^8].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `object`, the base class of every class; `typing.Any` is the unchecked dynamic type |
| JavaScript | idiom | no static types; most values inherit from `Object.prototype`, but primitives are not objects |
| TypeScript | native | `unknown`, which needs narrowing before use; `any`, which turns off checks |
| Java | native | `Object` is the root of the class hierarchy; primitive values must be boxed |
| Kotlin | native | `Any?` is the top type; `Any` is the root class and excludes `null` |
| C# | native | `object`, an alias for `System.Object`; value types are boxed when they become `object` |
| Go | native | `any`, an alias for `interface{}`, which every type implements |
| Rust | stdlib | no top type; `Box<dyn Any>` holds a value of a concrete type, and code can downcast it |
| C | idiom | `void *` points to data of any type, but the type is not stored |
| C++ | stdlib | no root class; `std::any` holds a copyable value of any type; `void *` as in C |
| Bash | n/a | every value is a string, so Bash has no hierarchy of types |
| PowerShell | native | values are .NET objects, so `[object]` holds any value |
| SQL | absent | each column has a declared type; SQL has no type for a value of any type |
| Regex | n/a | a pattern has no values |
| HTML | n/a | HTML has no types of values |
| CSS | n/a | CSS has no types of values in this sense |

Java, C#, Python and Kotlin have a universal base class, so the top type is also the type of an object at run time. Go and TypeScript have a static top type that is not a class. Rust and C++ have no top type in the language and give a library container that stores the type with the value, and C uses `void *`, which does not store the type.

The example files show these differences. Python shows that a type checker rejects operations on `object` and accepts them on `Any`. JavaScript shows that `typeof` and `instanceof Object` disagree for primitives. Java shows a `List<Object>`, boxing and a failed cast. C# shows boxing and pattern matching on `object`. Go shows `any` with a type switch and a type assertion. Rust shows `Box<dyn Any>` and `downcast_ref`. C shows a `void *` that the code must cast back to the right type. C++ shows `std::any` and `std::bad_any_cast`.

## Variations and related ideas

- **Top versus bottom.** The top type contains every value, and the [bottom type](/bottom-type/) contains none. A function that takes the top type accepts any argument, and a function that returns the bottom type never returns.
- **Top versus the dynamic type.** `Any` in Python and `any` in TypeScript accept every value and also fit where any type is expected, so the checker does not check them. A top type only goes up: a value of type `object` does not fit where a `str` is expected without a test.
- **Structural top types.** "In languages with a structural type system, the empty structure serves as a top type"[^1]. Go's empty interface and the empty object type `< >` of OCaml are examples[^1].
- **The top type before generics.** Before Java had generics, its collection classes held references of type `java.lang.Object`[^1], so each read needed a cast. Generic types, such as `List<String>`, replaced most of these uses. See [type erasure](/type-erasure/).

## Pitfalls

- **A downcast that fails.** A cast from the top type to a more specific type is checked only when the program runs. In Java, a wrong cast throws `ClassCastException`, and in Go, "If the type assertion is false, a run-time panic occurs"[^6]. The examples show both. Test the type first, with `instanceof`, a type switch or `isinstance`.
- **any instead of unknown.** In TypeScript, `any` turns off the checks, and in Python, "no type checking is performed when assigning a value of type Any to a more precise type"[^10]. Use `unknown` or `object` when the value can have any type but the code still needs checks.
- **Boxing.** In C#, "When you convert a value type variable to object, the value is boxed"[^3]. Boxing "allocates an object instance on the heap and copies the value"[^11], so a change to the copy does not change the original, as the C# example shows. Java has the same cost for `int` in a `List<Object>`.
- **Kotlin Any excludes null.** A parameter of type `Any` does not accept `null`. Use `Any?` for the top type[^1].
- **void * loses the type.** In C and C++, nothing stops a cast of a `void *` to the wrong type, and "downcasting from void* cannot be done in a safe way"[^1]. The result is undefined behavior, not an error.

## Cost

A variable of the top type must hold any value, so it usually holds a reference to an object, not the value itself. In C# and Java, a value of a primitive or value type is boxed when it becomes an `object`[^3], which takes a heap allocation[^11]. Each downcast is a test when the program runs[^1]. In Rust and C++, `Box<dyn Any>` and `std::any` store a type identifier with the value, so the downcast can check it[^9][^7]. In Python, every value is already an object, so the type `object` adds nothing.

## History

Smalltalk and JavaScript use `Object` as the name of their universal base class, and Java has `java.lang.Object`[^1]. Python made `object` the base of every class when it unified types and classes in version 2.2[^1]. Before Java had generics, its collection classes held references of type `java.lang.Object`[^1]. TypeScript added `unknown` as a checked partner of `any`[^1][^8], and Go added the predeclared name `any` for the empty interface[^6].

## References
